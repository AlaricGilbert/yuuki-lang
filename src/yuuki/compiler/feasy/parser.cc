#include <yuuki/compiler/feasy/parser.h>
namespace yuuki::compiler::feasy{
    using namespace yuuki::compiler::feasy::token;
    Parser::Parser(const std::shared_ptr <SyntaxContext> &context,
                   const std::shared_ptr <diagnostics::DiagnosticStream> &diagStream) {
        _context = context;
        _diagnosticStream = diagStream;
        _tokenIndex = 0;
    }
    std::shared_ptr<syntax::Expression> Parser::parseExpression() {
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
        return nullptr;
    }
}