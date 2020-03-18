#include <yuuki/compiler/feasy/parser.h>
#include <yuuki/compiler/feasy/token/token_util.h>
#include <yuuki/compiler/feasy/token/operator_util.h>
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
                    //parseImportDirective();
                    break;
                case TokenType::kw_namespace:
                    //parseNamespaceDeclaration();
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
                    //_context->syntaxTree->add(parseClass());
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

    std::shared_ptr<ClassDeclaration> Parser::parseClass(const std::shared_ptr<ModifierList>& modifiers) {
        _tokenIndex = getNextNotComment();
        auto name = parseName();
        std::size_t nextJudgeTokenIndex = getNextNotComment();
        std::shared_ptr<GenericDeclaration> genDecl;
        std::shared_ptr<InheritDeclaration> inheritDecl = std::make_shared<InheritDeclaration>();
        if(getTokenType(nextJudgeTokenIndex)== token::TokenType::op_less){
            _tokenIndex = nextJudgeTokenIndex;
            genDecl = parseGenericDeclaration();
            nextJudgeTokenIndex = getNextNotComment();
        } else{
            genDecl = std::make_shared<GenericDeclaration>(SyntaxNode::invalidTokenIndex);
        }
        if(getTokenType(nextJudgeTokenIndex)==TokenType::op_colon){
            _tokenIndex = nextJudgeTokenIndex;
            nextJudgeTokenIndex = getNextNotComment();
            parseNextInherit:
            if(getTokenType(nextJudgeTokenIndex)==TokenType::identifier){
                _tokenIndex = nextJudgeTokenIndex;
                inheritDecl->add(parseType());
                nextJudgeTokenIndex = getNextNotComment();
                if(getTokenType(nextJudgeTokenIndex)==TokenType::comma) {
                    _tokenIndex = nextJudgeTokenIndex;
                    nextJudgeTokenIndex = getNextNotComment();
                    goto parseNextInherit;
                }
                else if (getTokenType(nextJudgeTokenIndex)==TokenType::identifier){
                    _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected, _context->syntaxID)
                            .before(nextJudgeTokenIndex)
                            .message("',' expected")
                            .suggestion("insert ',' here")
                            .build();
                    goto parseNextInherit;
                }
            }
        }
        auto classDecl = std::make_shared<ClassDeclaration>(modifiers,name,genDecl,inheritDecl);
        auto errorHandle = [&]()->void{                       // we met a unexpected token, until we met one of:
            //
            // we should just jump over them and throw an error
            std::size_t startIndex = _tokenIndex;
            move({
                         TokenType::kw_class,
                         // generate modifiers movers
#define MODIFIER(X) TokenType::kw_##X,
#include <yuuki/compiler/feasy/token/tokens.inc>
                 });
            // we have moved into a position we can handle we should just push the error happened
            // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
            // TODO: push error 'UnexpectedTokens
        };
        switchForClassDeclType:
        nextJudgeTokenIndex = getNextNotComment();
        switch (getTokenType(nextJudgeTokenIndex)) {
            case TokenType::semi:
                classDecl->setSemiTokenIndex(_tokenIndex);
                break;
            case TokenType::l_brace: {
                _tokenIndex = nextJudgeTokenIndex;
                parseNextMember:
                nextJudgeTokenIndex = getNextNotComment();
                std::shared_ptr<ModifierList> modifiers = std::make_shared<ModifierList>();
                parseNextToken:
                switch (getTokenType(nextJudgeTokenIndex)) {
#define MODIFIER(X) case TokenType::kw_##X:
#include <yuuki/compiler/feasy/token/tokens.inc>
                        _tokenIndex = nextJudgeTokenIndex;
                        modifiers = parseModifiers();
                        goto parseNextToken;
                    case TokenType::kw_class:
                        _tokenIndex = nextJudgeTokenIndex;
                        classDecl->add(parseClass(modifiers));
                        goto parseNextMember;
                    case TokenType::identifier: {
                        std::size_t originTokenIndex = _tokenIndex;
                        if((!skipOverATypeDeclaration())||(getTokenType(getNextNotComment()))!=TokenType::identifier){
                            errorHandle();
                            goto parseNextToken;
                        }
                        _tokenIndex = getNextNotComment();
                        switch (getTokenType(getNextNotComment())){
                            case TokenType::op_equal:
                            case TokenType::semi:
                                _tokenIndex = originTokenIndex;
                                //classDecl->add(parse)
                            case TokenType::op_less:
                            case TokenType::l_brace:
                                _tokenIndex = originTokenIndex;
                                classDecl->add(parseMethodDeclaration(modifiers));
                            default:
                                errorHandle();
                                goto parseNextToken;
                        }

                    }
                    case TokenType::eof:
                        break;
                    default:
                        errorHandle();
                        goto parseNextToken;
                }
                break;
            }
            default:
                // we met a unexpected token, until we met one of:
                //
                // we should just jump over them and throw an error
                std::size_t startIndex = _tokenIndex;
                move({TokenType::semi, TokenType::l_brace});
                // we have moved into a position we can handle we should just push the error happened
                // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
                // TODO: push error 'UnexpectedTokens
                goto switchForClassDeclType;
        }
        return classDecl;
    }

    std::shared_ptr<GenericDeclaration> Parser::parseGenericDeclaration() {
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
            _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                    .before(nextJudgeTokenIndex)
                    .message("']' expected")
                    .suggestion("insert ']' here")
                    .build();
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
    std::shared_ptr<GenericArgumentList> Parser::parseGenericArgument() {
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
                _diagnosticStream << DiagnosticBuilder::error(CompileError::GreaterExpected,_context->syntaxID)
                        .before(_tokenIndex)
                        .message("'>' expected")
                        .suggestion("add '>' here")
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

    std::shared_ptr<MethodDeclaration>
    Parser::parseMethodDeclaration(const std::shared_ptr<ModifierList> &modifiers) {
        auto type = parseType();
        return nullptr;
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

    std::shared_ptr<syntax::Expression> Parser::parsePrecedenceExpression(std::list<token::TokenType> endTokens,
                                                                          int parentPrecedence) {
        std::size_t nextJudgeTokenIndex = getFirstNotComment();
        TokenType nextType = getTokenType(nextJudgeTokenIndex);
        std::shared_ptr<Expression> left;
        // we met a unary operator here, we can ensure that a unary expression can be directly parsed
        // since primary expressions had been directly handled with special codes, and primary expressions
        // are parsed locally so we can ensure that when we met a unary operator, unary has the highest
        // privilege
        if (OperatorUtil::isUnary(nextType) && OperatorUtil::unary <= parentPrecedence) {
            _tokenIndex = nextJudgeTokenIndex;
            _tokenIndex = getNextNotComment();
            left = parsePrecedenceExpression(endTokens, OperatorUtil::unary);
            left = std::make_shared<UnaryExpression>(nextType, nextJudgeTokenIndex, left);
        } else{
            _tokenIndex = nextJudgeTokenIndex;
            switch (nextType){
                case TokenType::l_paren: {
                    std::size_t lParenIndex = _tokenIndex;
                    _tokenIndex = getNextNotComment();
                    if (skipOverATypeDeclaration()) {
                        _tokenIndex = getNextNotComment();
                        if (getCurrentTokenType() == token::TokenType::r_paren) {
                            _tokenIndex = getNextNotComment();
                            if ((getCurrentTokenType() == token::TokenType::identifier) ||
                                    (getCurrentTokenType() == token::TokenType::l_paren)) {
                                // we ensured that in this situation, we met a explicit type convert expression

                                // reset token pos to start parse of skipped type
                                _tokenIndex = lParenIndex;
                                _tokenIndex = getNextNotComment();
                                auto type = parseType();
                                std::size_t rParenIndex = getNextNotComment();
                                _tokenIndex = rParenIndex;
                                _tokenIndex = getNextNotComment();
                                auto rExpr = parsePrecedenceExpression(endTokens, OperatorUtil::primary);
                                left = std::make_shared<ExplicitCastExpression>(lParenIndex, rParenIndex, type, rExpr);
                                break;
                            }
                        }
                    }
                    _tokenIndex = lParenIndex;
                    _tokenIndex = getNextNotComment();
                    auto newEndTokens = endTokens;
                    newEndTokens.push_back(TokenType::r_paren);
                    auto inner = parsePrecedenceExpression(newEndTokens);
                    nextJudgeTokenIndex = getNextNotComment();
                    nextType = getTokenType(nextJudgeTokenIndex);
                    if (nextType == token::TokenType::r_paren) {
                        left = std::make_shared<ParenthesesExpression>(lParenIndex, nextJudgeTokenIndex, inner);
                        _tokenIndex = nextJudgeTokenIndex;
                    } else {
                        left = std::make_shared<ParenthesesExpression>(lParenIndex, SyntaxNode::invalidTokenIndex,
                                                                       inner);
                    }
                    break;
                }
                case TokenType::identifier: {
                    left = std::make_shared<NameExpression>(parseName());
                    break;
                }
                case TokenType::kw_new: {
                    // TODO: handle with new expr;
                    auto type = parseType();
                    break;
                }
                default:
                    left = std::make_unique<NullExpression>();

                    if(OperatorUtil::getBinaryOperatorPrecedence(nextType)==OperatorUtil::initial){
                        std::size_t startIndex = _tokenIndex;
                        // we have moved into a position we can handle we should just push the error happened
                        // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
                        // TODO: push error 'UnexpectedTokens'
                        // unexpected tokens, move to endTokens;
                        move(endTokens);
                    }
                    break;
            }
        }
        while (true){
            nextJudgeTokenIndex = getNextNotComment();
            nextType = getTokenType(nextJudgeTokenIndex);
            // we met the end of the expression
            if(std::find(endTokens.begin(),endTokens.end(),nextType)!=endTokens.end())
                break;
            parsePrimary:

            switch (nextType){
                case token::TokenType::op_plusplus:
                case token::TokenType::op_minusminus:{
                    _tokenIndex = nextJudgeTokenIndex;
                    left = std::make_shared<PostfixExpression>(nextType,_tokenIndex,left);
                    break;
                }
                case token::TokenType::l_square: {
                    _tokenIndex = nextJudgeTokenIndex;
                    std::size_t lSquare = _tokenIndex;
                    _tokenIndex = getNextNotComment();
                    auto newEndTokens = endTokens;
                    newEndTokens.push_back(TokenType::r_square);
                    auto index = parsePrecedenceExpression(newEndTokens);
                    std::size_t rSquare = getNextNotComment();
                    if(getTokenType(rSquare)==TokenType::r_square){
                        left = std::make_shared<IndexExpression>(left,index,lSquare,rSquare);
                        _tokenIndex = rSquare;
                        nextJudgeTokenIndex = getNextNotComment();
                        nextType = getTokenType(nextJudgeTokenIndex);
                        if(nextType == TokenType::l_square)
                            goto parsePrimary;
                    } else{
                        left = std::make_shared<IndexExpression>(left,index,lSquare);
                        move(endTokens);
                        // TODO: throw ] expected
                    }
                    break;
                }
                case token::TokenType::l_paren:
                    break;
                default:
                    break;
            }
            // for context sensitive situations like
            //    obj.method<Type>()
            //              ^ should be parsed in primary precedence as '('.
            if(nextType == token::TokenType::op_less){

            }

            // we are now going to handle with binary expressions
            auto precedence = OperatorUtil::getBinaryOperatorPrecedence(nextType);
            if(precedence == OperatorUtil::initial)
                break;
            if(precedence >= parentPrecedence && precedence != OperatorUtil::assignment)
                break;
            _tokenIndex = nextJudgeTokenIndex;
            _tokenIndex = getNextNotComment();
            auto right = parsePrecedenceExpression(endTokens,precedence);
            left = std::make_shared<BinaryExpression>(left,nextType,nextJudgeTokenIndex,right);
        }
        return left;
    }

    bool Parser::skipOverATypeDeclaration() {
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
        std::size_t nextJudgeTokenIndex = _tokenIndex;
        if(getCurrentTokenType() == token::TokenType::identifier) {
            skipOverAName();
            nextJudgeTokenIndex = getNextNotComment();
        } else{
            return false;
        }
        switch (getTokenType(nextJudgeTokenIndex)){
            case token::TokenType::op_less:
                // move token to the '<' position
                _tokenIndex = nextJudgeTokenIndex;
                if (skipOverAGenericArgument()) {
                    nextJudgeTokenIndex = getNextNotComment();
                    if (getTokenType(nextJudgeTokenIndex) != token::TokenType::l_square)
                        return true;
                }
                return false;
            case token::TokenType::l_square:
                break;
            default:
                return true;
        }

        parseNextArray:
        _tokenIndex = nextJudgeTokenIndex;
        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex)!=token::TokenType::r_square){
            return false;
        }
        _tokenIndex = nextJudgeTokenIndex;

        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex) == token::TokenType::l_square){
            goto parseNextArray;
        }
        return true;
    }

    bool Parser::skipOverAGenericArgument() {
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
                return true;
            }
            case TokenType::eof:{
                _tokenIndex = nextJudgeTokenIndex;
                return false;
            }
            case TokenType::identifier:
            case TokenType::op_less:
            case TokenType::l_square:
                _tokenIndex = nextJudgeTokenIndex;
                skipOverATypeDeclaration();
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
                return false;
        }
    }

    bool Parser::skipOverAName() {
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
                return skipOverAName();
            } else{
                // since the next usable token is not a identifier, roll back.
                _tokenIndex = originalTokIndex;
            }
        }
        return true;
    }

}