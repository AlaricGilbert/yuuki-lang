#ifndef YUUKI_TOKEN_TOKEN_H
#define YUUKI_TOKEN_TOKEN_H

#include <yuuki/compiler/feasy/token/token_type.h>
#include <iostream>
namespace yuuki::compiler::feasy::token{
    /**
     * Represents a token in the source code.
     */
    class Token{
    public:
        /**
         * The type of the token.
         */
        TokenType        type = TokenType::undefined;
        /**
         * The relative position, compared to the start of the code.
         */
        std::size_t      offset = 0;
        /**
         * The raw code string_view which spilt from code by the lexer.
         */
        std::string_view rawCode;
        /**
         * Judge if the token matches the given type.
         * @param type: the type to be compared
         * @return true when type matches and false in other situations.
         */
        bool is(TokenType type);
    };
}
#endif //YUUKI_TOKEN_TOKEN_H