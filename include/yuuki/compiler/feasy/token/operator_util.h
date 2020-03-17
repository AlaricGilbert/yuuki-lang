#ifndef YUUKI_OPERATOR_UTIL_H
#define YUUKI_OPERATOR_UTIL_H

#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::token {
    class OperatorUtil {
    private:
        OperatorUtil() = delete; /* NOLINT */
    public:
        enum {
            // left-combined:
            primary = 1,
            unary,
            multiplicative,
            addition,
            shift,
            relational,
            equality,
            logical_and,
            logical_xor,
            logical_or,
            conditional_and,
            conditional_or,
            // right-combined:
            conditional,
            assignment,
            // initial
            initial
        };
        static bool isUnary(TokenType type);
        static bool isPrimary(TokenType type);
        static std::size_t getBinaryOperatorPrecedence(TokenType type);
    };
}
#endif //YUUKI_OPERATOR_UTIL_H
