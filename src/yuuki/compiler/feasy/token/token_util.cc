#include <string>
#include "yuuki/compiler/feasy/token/token_util.h"

namespace yuuki::compiler::feasy::token{
    const char * TokenUtil::getSpell(TokenType type){
        switch (type){
#define PUNCTUATOR(X,Y) case X: return Y;
#define KEYWORD(X) case kw_ ## X: return #X;
#define OPERATOR(X,Y) case op_ ## X: return Y;
#define MODIFIER(X) case kw_ ## X: return #X;
#include "yuuki/compiler/feasy/token/tokens.inc"

            case unknown:
                return "unknown";
            case undefined:
                return "undefined";
            case modifiers:
                return "modifiers";
            case punctuator:
                return "punctuator";
            case keywords:
                return "keywords";
            case operators:
                return "operators";
            case interline_comment:
                return "interline_comment";
            case inline_comment:
                return "inline_comment";
            case identifier:
                return "identifier";
            case eof:
                return "eof";
            default:
                return nullptr;
        }
    }

    TokenType TokenUtil::getType(const std::string& spell){
        if(G_AllTokenSpellMap.count(spell)==0){
            return token::unknown;
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

