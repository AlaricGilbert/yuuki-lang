#ifndef YUUKI_PARSER_H
#define YUUKI_PARSER_H

#include <memory>
#include <yuuki/compiler/diagnostics/diagnostic.h>
#include <yuuki/compiler/feasy/syntax_context.h>
#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/syntax/identifier_expression.h>
#include <yuuki/compiler/feasy/syntax/unary_expression.h>
namespace yuuki::compiler::feasy{
    /**
     * Syntax analyser for yuuki language.
     */
    class Parser{
    public:
        /**
         * Constructs a lexer from given syntax analyse context and diagnostic info reporter.
         * @param context:  the compile context during lexical analyse and syntax analyse
         * @param diagInfo: the diagnostic info collector to report errors to
         */
        Parser(const std::shared_ptr<SyntaxContext> &context, const std::shared_ptr<diagnostics::Diagnostic> &diagInfo);

        std::shared_ptr<syntax::Expression> parseExpression();
    private:
        // copy of the context
        std::shared_ptr<SyntaxContext> _context ;
        // copy of the diagnostic info collector
        std::shared_ptr<diagnostics::Diagnostic> _diagnostic;
        // the index of the current token
        size_t _tokenIndex;
    };
}
#endif //YUUKI_PARSER_H
