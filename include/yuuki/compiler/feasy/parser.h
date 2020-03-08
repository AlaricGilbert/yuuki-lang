#ifndef YUUKI_PARSER_H
#define YUUKI_PARSER_H

#include <memory>
#include <yuuki/compiler/diagnostics/diagnostic_stream.h>
#include <yuuki/compiler/feasy/syntax_context.h>
#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/namespace_declaration.h>
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
         * @param diagnosticStream: the diagnostic info collector to report errors to
         */
        Parser(const std::shared_ptr<SyntaxContext> &context, const std::shared_ptr<diagnostics::DiagnosticStream> &diagStream);

        void parse();

        void parseImportDirective();
        void parseNamespaceDeclaration();
        void parseClassDeclaration(const std::shared_ptr<syntax::ISyntaxList<syntax::ClassDeclaration>>& parent);
        void parseGenericDeclaration();
        void parseExpression();
        std::shared_ptr<syntax::Name> parseName();
        std::shared_ptr<syntax::ModifierList> parseModifiers();
        inline void tryJumpOverComments();
    private:
        void move(std::initializer_list<token::TokenType> acceptableEndTokType);
        // copy of the context
        std::shared_ptr<SyntaxContext> _context ;
        // copy of the diagnostic info collector
        std::shared_ptr<diagnostics::DiagnosticStream> _diagnosticStream;
        // the index of the current token
        size_t _tokenIndex;
    };
}
#endif //YUUKI_PARSER_H
