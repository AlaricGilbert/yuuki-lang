#include <yuuki/compiler/feasy/parser.h>
namespace yuuki::compiler::feasy{
    Parser::Parser(const std::shared_ptr <SyntaxContext> &context,
                   const std::shared_ptr <diagnostics::Diagnostic> &diagInfo) {
        _context = context;
        _diagnostic = diagInfo;
        _tokenIndex = 0;
    }


}