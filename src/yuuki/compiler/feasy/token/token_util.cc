#include <string>
#include "yuuki/compiler/feasy/token/token_util.h"

namespace yuuki::compiler::feasy::token{
    const char * TokenUtil::getSpell(TokenType type){
        switch (type){
#define PUNCTUATOR(X,Y) case TokenType::X: return Y;
#define KEYWORD(X) case TokenType::kw_ ## X: return #X;
#define OPERATOR(X,Y) case TokenType::op_ ## X: return Y;
#define MODIFIER(X) case TokenType::kw_ ## X: return #X;
#include "yuuki/compiler/feasy/token/tokens.inc"

            case TokenType::unknown:
                return "unknown";
            case TokenType::undefined:
                return "undefined";
            case TokenType::modifiers:
                return "modifiers";
            case TokenType::punctuator:
                return "punctuator";
            case TokenType::keywords:
                return "keywords";
            case TokenType::operators:
                return "operators";
            case TokenType::interline_comment:
                return "interline_comment";
            case TokenType::inline_comment:
                return "inline_comment";
            case TokenType::identifier:
                return "identifier";
            case TokenType::eof:
                return "eof";
            default:
                return nullptr;
        }
    }

    TokenType TokenUtil::getType(const std::string& spell){
        if(G_AllTokenSpellMap.count(spell)==0){
            return TokenType::unknown;
        }
        return G_AllTokenSpellMap.at(spell);
    }

    TokenType TokenUtil::getType(char c) {
        std::string p;
        p.push_back(c);

        return getType(p);
    }

    bool TokenUtil::isPunctuator(TokenType type) {
        return G_PunctuatorSet.count(type) ==1;
    }

    bool TokenUtil::isModifier(TokenType type) {
        return G_ModifierSet.count(type) ==1;
    }

    bool TokenUtil::isKeyword(TokenType type) {
        return G_KeywordSet.count(type) ==1;
    }

    bool TokenUtil::isOperator(TokenType type) {
        return G_OperatorSet.count(type) ==1;
    }

}

