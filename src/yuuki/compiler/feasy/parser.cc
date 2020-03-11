#include <yuuki/compiler/feasy/parser.h>
#include <yuuki/compiler/feasy/token/token_util.h>
#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <yuuki/compiler/diagnostics/diagnostic_builder.h>
namespace yuuki::compiler::feasy{
    using namespace yuuki::compiler::diagnostics;
    using namespace yuuki::compiler::feasy::token;
    using namespace yuuki::compiler::feasy::syntax;
    Parser::Parser(const std::shared_ptr <SyntaxContext> &context,
                   const std::shared_ptr <DiagnosticStream> &diagStream) {
        _context = context;
        _diagnosticStream = diagStream;
        _tokenIndex = 0;
    }

    void Parser::parseExpression() {
        static_assert(std::is_base_of<Parser,Parser>::value);
        // check for unary operators
        switch (_context->tokens[_tokenIndex]->type){
            case TokenType::op_minus:
            case TokenType::op_minusminus:
            case TokenType::op_plus:
            case TokenType::op_plusplus:
            case TokenType::op_tilde:
            case TokenType::op_exclaim:
            case TokenType::l_paren:
            case TokenType::identifier:
            case TokenType::numeric_const:
            case TokenType::string_const:
            case TokenType::false_const:
            case TokenType::true_const:
            default:
                break;
        }
    }

    void Parser::parse() {
        // the recovery lambda for main parse section
        auto recover = [&]()->void{
            move({
                         TokenType::kw_import,
                         TokenType::kw_namespace,
                         // generate modifiers movers
#define MODIFIER(X) TokenType::kw_##X,
#include <yuuki/compiler/feasy/token/tokens.inc>
                         TokenType::inline_comment,
                         TokenType::interline_comment,
                         TokenType::semi
                 });
        };
        while (_tokenIndex < _context->tokens.size()){
            switch (_context->tokens[_tokenIndex]->type){
                // we are going to handle the main situations in the global syntax unit
                case TokenType::kw_import:
                    parseImportDirective();
                    break;
                case TokenType::kw_namespace:
                    parseNamespaceDeclaration();
                    break;
                // we are going to handle modifiers which may be added to tokens.inc in the future design!
                // so we use marcos to generate modifier list, codes are generated like:
                //     case TokenType::kw_public:
                #define MODIFIER(X) case TokenType::kw_##X:
                #include <yuuki/compiler/feasy/token/tokens.inc>
                // the modifiers in the global environment only results lead to a global namespaced class
                // so we can merge the situation with the kw_class
                case TokenType::kw_class:
                {
                    _context->syntaxTree->add(parseClass());
                    if(_context->tokens[_tokenIndex])
                    break;
                }
                case TokenType::identifier: {
                    // we just can't decide what happened here, we should firstly parse the full name
                    // since we may meet 'some.situation.like.this'
                    auto firstIdIndex = _tokenIndex;
                    auto name = parseName();

                    // we are going to find out what situation should be:
                    //      some.libs; -> import some.libs;
                    //
                    //      when next line is like import some.other.libs; or namespace some.ns{
                    //
                    //      some {     -> class some {
                    //
                    //      some.lib { -> namespace  {
                    switch (_context->tokens[_tokenIndex]->type){
                        case TokenType::semi: {
                            // we met the first situation, think it's a import directive and push an error
                            auto import = std::make_shared<ImportDirective>(std::move(name),
                                                                            SyntaxNode::invalidTokenIndex,
                                                                            _tokenIndex);
                            _context->syntaxTree->add(import);
                            _diagnosticStream << DiagnosticBuilder::
                            error(CompileError::ImportExpected, _context->syntaxID)
                                    .before(firstIdIndex)
                                    .message("import keyword expected")
                                    .suggestion("insert 'import' here")
                                    .build();
                            break;
                        }
                        case TokenType::l_brace:{
                            // it's a start of a block
                            if(name->getType() == SyntaxType::IdentifierName){
                                // tend to think it's a class
                                // TODO: make a class
                            } else{
                                // name is a Qualified name, must be a namespace.
                                // TODO: make a namespace
                            }

                        }
                        default:
                            // unexpected situation
                            recover();
                            // TODO: push error 'UnexpectedTokens'
                            break;
                    }
                    break;
                }
                // we are going to handle comments
                case TokenType::inline_comment:
                case TokenType::interline_comment:
                    // just jump over the comments for now
                // then the other special situations
                case TokenType::semi:
                    // we think a single semi is a null directive
                    // which would not be constructed into parse tree
                    _tokenIndex++;
                    break;
                case TokenType::eof:
                    // ends the parsing when met the end of the token list
                    return;
                default:
                    // we met a unexpected token, until we met one of:
                    //      import, namespace, //, /**/, ;, modifiers
                    // we should just jump over them and throw an error
                    std::size_t startIndex = _tokenIndex;
                    recover();
                    // we have moved into a position we can handle we should just push the error happened
                    // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
                    // TODO: push error 'UnexpectedTokens'
                    break;
            }
        }
    }

    void Parser::parseImportDirective() {

    }

    void Parser::parseNamespaceDeclaration() {

    }

    std::shared_ptr<Name> Parser::parseName() {
        // call to this function should ensure that token _context->tokens[_tokenIndex] is a identifier!!
        auto currName = parseIdentifier();
        // since call won't move over the identifier, we should move here
        auto nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(getNextNotComment()) == TokenType::op_period){
            // record the current token index (the index BEFORE period)
            auto originalTokIndex = _tokenIndex;
            // _tokenIndex is now set to
            _tokenIndex = nextJudgeTokenIndex;
            // move to next non-comment token position
            nextJudgeTokenIndex = getNextNotComment();
            if(getTokenType(nextJudgeTokenIndex) == TokenType::identifier) {
                _tokenIndex = nextJudgeTokenIndex;
                // continue to parse right part of name
                auto rightName = parseName();
                return std::make_shared<QualifiedName>(std::move(currName), _tokenIndex, rightName);
            } else{
                // since the next usable token is not a identifier, roll back.
                _tokenIndex = originalTokIndex;
            }
        }
        return currName;
    }

    std::shared_ptr<ModifierList> Parser::parseModifiers() {
        // call to this function should ensure that token _context->tokens[_tokenIndex] is a modifier!!
        auto modifiers = std::make_shared<ModifierList>();
        do {
            modifiers->add(std::make_shared<ModifierMark>(_context->tokens[_tokenIndex]->type, _tokenIndex));
            jumpOverComments();
        } while (_context->tokens[++_tokenIndex]->is(TokenType::modifiers));
        return modifiers;
    }

    std::shared_ptr<ClassDeclaration> Parser::parseClass() {
        return std::shared_ptr<syntax::ClassDeclaration>();
    }

    std::shared_ptr<GenericDeclaration> Parser::parseGenericInfo() {
        auto genericDecl = std::make_shared<GenericDeclaration>(_tokenIndex++);
        parseNextGenericDeclArg:
        jumpOverComments();
        switch (getCurrentTokenType()){
            case TokenType::identifier: {
                genericDecl->add(parseIdentifier());
                moveAndJumpOverComments();
                // we are going to handle situations:
                //    <...TParsed ,                 , what expected for next
                //                ^
                //    <...TParsed TNotParsed        , push 'comma' expected
                //                    ^
                //    <...TParsed >                 , end generic info parsing
                //                ^
                // so we look forward for next:
                switch (getCurrentTokenType()) {
                    case TokenType::comma:
                        _tokenIndex++;
                        goto parseNextGenericDeclArg;
                    case TokenType::identifier:
                        // push an error immediately and parse next identifier
                        _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected, _context->syntaxID)
                                .after(_tokenIndex - 1)    // we should push an error after last identifier
                                .message("expected ',' between two identifiers")
                                .suggestion("add ',' here")
                                .build();
                        goto parseNextGenericDeclArg;
                    case TokenType::op_greater:
                        break;
                    default:
                        goto errorHandle;
                }
                break;
            }
            case TokenType::comma:{
                // we are going to handle situations:
                //    < ,...
                //     ^                    identifier expected
                //or  <...TParsed, ,...>
                //                ^         identifier expected
                // and we are at the token index of the comma before '...>'
                _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                        .before(_tokenIndex)    // we should push an error before current token
                        .message("expected identifier")
                        .build();
                _tokenIndex++;
                goto parseNextGenericDeclArg;
            }
            case TokenType::op_greater:{
                // we are going to handle situations:
                //    < >
                //     ^         identifier expected
                //    <..., >
                //         ^     identifier expected
                // and both of the situations will end the parse
                _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                        .before(_tokenIndex)    // we should push an error before current token
                        .message("expected identifier")
                        .build();
                break;
            }
            default:
                goto errorHandle;
        }
        genericDecl->setEndOpIndex(_tokenIndex);
        return genericDecl;
        errorHandle:
        auto errorStartTok = _tokenIndex;
        // other unexpected token move to '>' or any punctuator in any error situation
        move({
            TokenType::op_greater,
            TokenType::op_colon,
#define PUNCTUATOR(X,Y) TokenType::X,
#include <yuuki/compiler/feasy/token/tokens.inc>
        });
        if(getCurrentTokenType() == TokenType::op_colon){
            genericDecl->setEndOpIndex(_tokenIndex);
        }
        // we have moved into a position we can handle we should just push the error happened
        // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
        // TODO: push error 'UnexpectedTokens'
        return genericDecl;
    }

    std::shared_ptr<syntax::IdentifierName> Parser::parseIdentifier() {
        // call to this function should ensure that token _context->tokens[_tokenIndex] is a identifier!!
        return std::make_shared<IdentifierName>(
                (std::string) _context->tokens[_tokenIndex]->rawCode, _tokenIndex);
    }

    // parseType and parseGenericArg should be implemented in the same time
    std::shared_ptr<syntax::Type> Parser::parseType() {
        // a valid type should start with a name like
        //     Generic<G.T.S>
        //        ^                         valid!
        // the judge token in this situation should be next non-comment token (marked by '^')
        //     Generic /*interline comment*/ <G.T.S>
        //                                   ^
        // but in some error cases a type could start just like
        //     Generic<T1, [ ]>
        //                 ^                identifier expected,   parse as Generic<T1, unknown[]>
        //     Generic<T1, <SomeArgument>>
        //                 ^                identifier expected,   parse as Generic<T1, unknown<SomeArgument>>
        //     Generic<T1,      +>
        //                 ↑    ^           unexpected identifier,
        //                 |                identifier expected,   parse as Generic<T1>
        // the judge token in this situation should be current token (marked by '^')
        std::shared_ptr<Type> type;
        std::size_t nextJudgeTokenIndex = _tokenIndex;
        if(getCurrentTokenType() == token::TokenType::identifier) {
            type = std::make_shared<TrivialType>(parseName());
            nextJudgeTokenIndex = getNextNotComment();
        } else{
            // TODO:we should make a identifier expected error here
            type = std::make_shared<UnknownType>();
        }
        switch (getTokenType(nextJudgeTokenIndex)){
            case token::TokenType::op_less:
                // move token to the '<' position
                _tokenIndex = nextJudgeTokenIndex;
                type = std::make_shared<GenericType>(type,parseGenericArgument());
                nextJudgeTokenIndex = getNextNotComment();
                if(getTokenType(nextJudgeTokenIndex) != token::TokenType::l_square)
                    return type;
                break;
            case token::TokenType::l_square:
                break;
            default:
                return type;
        }

        parseNextArray:
        _tokenIndex = nextJudgeTokenIndex;
        type = std::make_shared<ArrayType>(type,_tokenIndex);
        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex)!=token::TokenType::r_square){
            // TODO: throw r_square expected!
            return type;
        }
        _tokenIndex = nextJudgeTokenIndex;
        std::static_pointer_cast<ArrayType>(type)->setRSquareIndex(_tokenIndex);   // []

        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex) == token::TokenType::l_square){
            goto parseNextArray;
        }
        return type;
    }

    // warning: this function may make change to _context->tokens
    std::shared_ptr<syntax::GenericArgumentList> Parser::parseGenericArgument() {
        // generic argument should start with '<'
        auto arguments = std::make_shared<GenericArgumentList>(_tokenIndex);
        auto nextJudgeTokenIndex = getNextNotComment();
        // for situations like  Generic< >
        //                              ^    identifier expected
        parseNextTypeArg:
        switch (getTokenType(nextJudgeTokenIndex)){
            case TokenType::op_greatergreater:
                _tokenIndex = nextJudgeTokenIndex;
                splitCurrentMultiCharOperator();
            case TokenType::op_greater:{
                _tokenIndex = nextJudgeTokenIndex;
                if(!arguments->hasChild()) {
                    _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                            .before(_tokenIndex)
                            .message("identifier expected")
                            .build();
                }
                arguments->setEndOpIndex(_tokenIndex);
                return arguments;
            }
            case TokenType::eof:{
                _tokenIndex = nextJudgeTokenIndex;
                _diagnosticStream << DiagnosticBuilder::error(CompileError::RSquareExpected,_context->syntaxID)
                        .before(_tokenIndex)
                        .message("'{' expected")
                        .suggestion("add '{' here")
                        .build();
                return arguments;
            }
            case TokenType::identifier:
            case TokenType::op_less:
            case TokenType::l_square:
                _tokenIndex = nextJudgeTokenIndex;
                arguments->add(parseType());
                switch (getCurrentTokenType()){
                    case TokenType::identifier:
                    case TokenType::op_greater:
                    case TokenType::r_square:
                        nextJudgeTokenIndex = getNextNotComment();
                        break;
                    default:
                        nextJudgeTokenIndex = _tokenIndex;
                        break;
                }
                if(getTokenType(nextJudgeTokenIndex) == TokenType::comma){
                    _tokenIndex = nextJudgeTokenIndex;
                    nextJudgeTokenIndex = getNextNotComment();
                }
                goto parseNextTypeArg;

            default:
                std::size_t startIndex = _tokenIndex;
                move({TokenType::op_greater,TokenType::op_greatergreater});

                // we have moved into a position we can handle we should just push the error happened
                // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
                // TODO: push error 'UnexpectedTokens'
                nextJudgeTokenIndex = _tokenIndex;
                goto parseNextTypeArg;
        }
    }

    void Parser::splitCurrentMultiCharOperator() {
        std::string_view code = _context->tokens[_tokenIndex]->rawCode;
        std::size_t offset = _context->tokens[_tokenIndex]->offset;
        _context->tokens.erase(_context->tokens.begin() + _tokenIndex);
        for(std::size_t i = 0; i < code.size();i++) {
            auto tok = std::make_unique<Token>();
            tok->rawCode = code.substr( i, 1);
            tok->offset = offset + i;
            tok->type = TokenUtil::getType(code[i]);
            _context->tokens.insert(_context->tokens.begin() + _tokenIndex + i, std::move(tok));
        }
    }
}