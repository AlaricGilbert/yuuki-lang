#include <yuuki/compiler/feasy/syntax/unary_expression.h>

#include <utility>
#include <rang/rang.h>
#include <yuuki/compiler/feasy/token/token_util.h>

namespace yuuki::compiler::feasy::syntax{
    void UnaryExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_right, true);
    }

    UnaryExpression::UnaryExpression(token::TokenType operatorType,
                                     const std::shared_ptr<SyntaxNode>& right) {
        _right = right;
        _operatorType = operatorType;
    }

    token::TokenType UnaryExpression::getOperatorType() {
        return _operatorType;
    }

    void UnaryExpression::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "UnaryExpression "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::green
              << '\'' << token::TokenUtil::getSpell(_operatorType) << '\''
              << rang::fg::reset <<std::endl;
        } else{
            s << "UnaryExpression "
              << "<" << this << "> "
              << '\'' << token::TokenUtil::getSpell(_operatorType) << '\'' << std::endl;
        }
    }

    void UnaryExpression::analyseType() {

    }

    SyntaxType UnaryExpression::getType() {
        return SyntaxType::UnaryExpression;
    }

    bool UnaryExpression::hasChild() {
        return true;
    }
}