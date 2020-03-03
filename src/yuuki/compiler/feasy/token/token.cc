#include "yuuki/compiler/feasy/token/token.h"
namespace yuuki::compiler::feasy::token{
    bool Token::is(TokenType rhs_type) {
        switch (rhs_type) {
            case modifiers:
                return G_ModifierSet.count(type) == 1;
            case punctuator:
                return G_PunctuatorSet.count(type) == 1;
            case keywords:
                return G_KeywordSet.count(type) == 1;
            case operators:
                return G_OperatorSet.count(type) == 1;
            default:
                return type == rhs_type;
        }
    }
}

