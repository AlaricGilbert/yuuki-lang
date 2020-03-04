#ifndef YUUKI_OPERATOR_UTIL_H
#define YUUKI_OPERATOR_UTIL_H

#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::token {
    class OperatorUtil {
    private:
        OperatorUtil() = delete; /* NOLINT */
    public:
        enum class OpType{
            primary = 1,
            unary = 2,
            multiplicative = 3,
            addition = 4,
            shift = 5,
            relational = 6,
            equality = 7,
            logical_and = 8,
            logical_xor = 9,
            logical_or = 10,
            conditional_and = 11,
            conditional_or = 12,
            conditional = 13,
            assignment = 14
        };
        static OpType getType(TokenType t);
    };
}
#endif //YUUKI_OPERATOR_UTIL_H
