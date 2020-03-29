#include <yuuki/compiler/feasy/syntax/unary_expression.h>

#include <utility>
#include <rang/rang.h>
#include <yuuki/compiler/feasy/token/token_util.h>

namespace yuuki::compiler::feasy::syntax{
    void UnaryExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_right, true);
    }

    UnaryExpression::UnaryExpression(token::TokenType operatorType,
                                     std::size_t opIndex,
                                     const std::shared_ptr<Expression>& right) {
        _right = right;
        _opIndex = opIndex;
        _operatorType = operatorType;
    }

    token::TokenType UnaryExpression::getOperatorType() {
        return _operatorType;
    }

    void UnaryExpression::writeCurrentInfo(std::ostream &s) {
        Expression::writeCurrentInfo(s);
    }

    void UnaryExpression::analyseType() {

    }

    SyntaxType UnaryExpression::getType() {
        return SyntaxType::UnaryExpression;
    }

    bool UnaryExpression::hasChild() {
        return true;
    }

    std::size_t UnaryExpression::start() {
        return _opIndex;
    }

    std::size_t UnaryExpression::end() {
        if (_right->getType() != SyntaxType::NullExpression)
            return _right->end();
        return _opIndex;
    }

    std::string UnaryExpression::toString() {
        return token::TokenUtil::getSpell(_operatorType) + _right->toString();
    }

    std::size_t UnaryExpression::getOpIndex() {
        return _opIndex;
    }

    std::shared_ptr<Expression> UnaryExpression::getRight() {
        return _right;
    }
}