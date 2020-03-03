#ifndef YUUKI_TOKEN_TOKENUTIL_H
#define YUUKI_TOKEN_TOKENUTIL_H

#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::token{
    /**
     * A static utility which provides basic operations of tokens.
     */
    class TokenUtil
    {
    public:
        /**
         * Get the spell of given token type.
         * @param type
         * @return
         */
        static const char * getSpell(TokenType type);
        /**
         * Judge if the given token type is a punctuator
         * @param type
         * @return
         */
        static bool isPunctuator(TokenType type);
        /**
         * Judge if the given token type is a modifier
         * @param type
         * @return
         */
        static bool isModifier(TokenType type);
        /**
         * Judge if the given token type is a keyword
         * @param type
         * @return
         */
        static bool isKeyword(TokenType type);
        /**
         * Judge if the given token type is a operator
         * @param type
         * @return
         */
        static bool isOperator(TokenType type);
        /**
         * Get the type by the given token spell
         * @param spell
         * @return
         */
        static TokenType getType(const std::string& spell);
        /**
         * Get the type by the given token spell
         * @param c
         * @return
         */
        static TokenType getType(char c);
    private:
        TokenUtil() = delete; /* NOLINT */
        /* data */
    };
    
    
}
#endif //YUUKI_TOKEN_TOKENUTIL_H