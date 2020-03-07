#include <yuuki/compiler/feasy/parser.h>
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
                    // these modifiers marks a global declaration, a warning should be pushed in semantic phase
                    // TODO: parse modifiers
                    _tokenIndex++;
                    break;
                case TokenType::identifier: {
                    // we just can't decide what happened here, we should firstly parse the full name
                    // since we may meet 'some.situation.like.this'
                    auto name = parseName();

                    // we met a new choice:
                    //
                    switch (_context->tokens[_tokenIndex]->type){
                        default:
                            break;
                    }
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
                    move({
                        TokenType::kw_import,
                        TokenType::kw_namespace,
                        // generate modifiers
                        #define MODIFIER(X) TokenType::kw_##X,
                        #include <yuuki/compiler/feasy/token/tokens.inc>
                        TokenType::inline_comment,
                        TokenType::interline_comment,
                        TokenType::semi
                    });
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

    void Parser::move(std::initializer_list<TokenType> acceptable) {
        TokenType lastType;
        do{
            lastType = _context->tokens[_tokenIndex++]->type;
        }while ((std::find(acceptable.begin(),acceptable.end(),lastType)==acceptable.end())
              &&(lastType != TokenType::eof));
        _tokenIndex--;
    }

    std::shared_ptr<Name> Parser::parseName() {
        // call to this function should ensure that token _context->tokens[_tokenIndex] is a identifier!!
        auto currName =std::make_shared<IdentifierName>(
                (std::string)_context->tokens[_tokenIndex]->rawCode,_tokenIndex);
        // since current token is NOT eof, so next token must exists, we do not have to check _tokenIndex < tokens.size()
        if(_context->tokens[++_tokenIndex]->type == TokenType::op_period){
            // same as above, we do not have to check _tokenIndex < tokens.size()
            // check for next token type.
            if(_context->tokens[_tokenIndex+1]->type == TokenType::identifier) {
                // record the _tokenIndex BEFORE we make change to it.
                auto periodIndex = _tokenIndex;
                _tokenIndex++;
                // continue to parse right part of name
                auto rightName = parseName();
                return std::make_shared<QualifiedName>(std::move(currName), periodIndex, rightName);
            }
        }
        return currName;
    }
}