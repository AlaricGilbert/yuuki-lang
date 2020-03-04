#ifndef YUUKI_TOKEN_TOKENTYPE_H
#define YUUKI_TOKEN_TOKENTYPE_H
#include <map>
#include <set>
#include <string>

namespace yuuki::compiler::feasy::token{
    /**
     * A enum type for defining types of token, by using hack of marcos to generate codes.
     */
    enum class TokenType: unsigned short {
#define TOKEN(X) X,
#include "tokens.inc"
        undefined  ,
        modifiers  ,
        punctuator ,
        operators  ,
        keywords   
    };

    /**
     * A global const map which stores the relation of all the types of TOKEN and their spells.
     */
    const std::map<std::string,TokenType> G_AllTokenSpellMap = { /* NOLINT */
#define PUNCTUATOR(X,Y) {Y,TokenType::X},
#define KEYWORD(X) {#X,TokenType::kw_ ## X},
#define OPERATOR(X,Y) {Y,TokenType::op_ ## X},
#define MODIFIER(X) {#X,TokenType::kw_ ## X},
#include "tokens.inc"
    };

    /**
     * A global const map which stores the relation of all the types of PUNCTUATOR and their spells.
     */
    const std::set<TokenType> G_PunctuatorSet = { /* NOLINT */
#define PUNCTUATOR(X,Y) TokenType::X,
#include "tokens.inc"
    };

    /**
     * A global const map which stores the relation of all the types of KEYWORD and their spells.
     */
    const std::set<TokenType> G_KeywordSet = { /* NOLINT */
#define KEYWORD(X) TokenType::kw_ ## X,
#include "tokens.inc"
    };

    /**
     * A global const map which stores the relation of all the types of OPERATOR and their spells.
     */
    const std::set<TokenType> G_OperatorSet = { /* NOLINT */
#define OPERATOR(X,Y) TokenType::op_ ## X,
#include "tokens.inc"
    };

    /**
     * A global const map which stores the relation of all the types of MODIFIER and their spells.
     */
    const std::set<TokenType> G_ModifierSet = { /* NOLINT */
#define MODIFIER(X) TokenType::kw_ ## X,
#include "tokens.inc"
    };
}

#endif //YUUKI_TOKEN_TOKENTYPE_H