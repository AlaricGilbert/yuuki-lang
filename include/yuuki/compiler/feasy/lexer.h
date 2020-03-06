#ifndef YUUKI_LEXER_H
#define YUUKI_LEXER_H

#include <yuuki/compiler/diagnostics/diagnostic_stream.h>
#include <yuuki/compiler/feasy/token/token.h>
#include <yuuki/compiler/feasy/syntax_context.h>
#include <string_view>

namespace yuuki::compiler::feasy{
    /**
     * Lexical analyser for yuuki language
     */
    class Lexer {
    public:
        /**
         * Constructs a lexer from given syntax analyse context and diagnostic info reporter.
         *
         * The life of the raw codes of the tokens lexed by this lexer is decided by the life time of the life time of
         * member 'code' of the given parameter 'context'. Result of accessing to context's tokens should be unexpected
         * after the content of the code have been collected.
         * @param context:  the compile context during lexical analyse and syntax analyse
         * @param diagnosticStream: the diagnostic info collector to report errors to
         */
        explicit Lexer(const std::shared_ptr<SyntaxContext> &context,
                const std::shared_ptr<diagnostics::DiagnosticStream> &diagnosticStream);

        /**
         * Lex all tokens from given code string and store all into context.
         *
         * This will directly write all tokens into context.
         */
        bool lex();
    private:
        // copy of the code
        std::string_view _code;
        // copy of the context
        std::shared_ptr<SyntaxContext> _context ;
        // copy of the diagnostic info collector
        std::shared_ptr<diagnostics::DiagnosticStream> _diagnosticStream;
        // current relative position to the first char in the _code variable
        size_t _position;
        // marks if the lexer had met the end of the code file
        bool _eof;
        // lex a token and write it into context
        bool lexToken();
        // continue to lex a string const
        bool lexStringConst();
        // continue to lex a numeric const
        bool lexNumericConst();
        // continue to lex a identifier
        bool lexIdentifier(size_t index = 0);
        // continue to lex a operator
        bool lexOperator();
        // continue to lex a inline comment
        bool lexInlineComment();
        // continue to lex a interline comment
        bool lexInterlineComment();
        bool isUnicodeCharValidOfIdentifier();
        bool isCurrCharSplitter();
    };
}
#endif //YUUKI_LEXER_H
