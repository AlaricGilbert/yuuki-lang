#ifndef YUUKI_SYNTAX_CONTEXT_H
#define YUUKI_SYNTAX_CONTEXT_H

#include <yuuki/compiler/feasy/code_postion.h>
#include <yuuki/compiler/feasy/token/token.h>
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <string_view>
#include <vector>
namespace yuuki::compiler::feasy{
    /**
     * An single source-file syntax representation to store analyse mid products
     * including lex and parse phase.
     */
    class SyntaxContext{
    public:
        /**
         * The path of the given code, used in diagnostics.
         */
        std::string codePath;
        /**
         * The code reference string.
         */
        std::string_view code;
        /**
         * The reference to the lines of the codes.
         */
        std::vector<std::string_view> lines;
        /**
         * The tokens lexed by Lexer are stored here.
         */
        std::vector<std::unique_ptr<token::Token>> tokens;
        /**
         * The index of the current syntax context.
         */
        std::size_t syntaxID;
        SyntaxContext(const std::string_view &code, std::size_t syntaxID);

        /**
         * Locate the given token in the source code.
         * @param token
         * @return
         */
        CodePosition locate(const token::Token& token);
        /**
         * Locate the token defined by the index in the source code.
         * @param index
         * @return
         */
        CodePosition locate(std::size_t index);

    private:
        // used for locate tokens
        std::vector<std::size_t> lineStartPos;
    };
}
#endif //YUUKI_SYNTAX_CONTEXT_H
