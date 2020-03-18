#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/token/token_util.h>
#include <utility>
#include <rang/rang.h>
#include <yuuki/compiler/feasy/syntax/unary_expression.h>

namespace yuuki::compiler::feasy::syntax{
    void BinaryExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)> &syntaxWalker) {
        syntaxWalker(_left, false);
        syntaxWalker(_right, true);
    }

    BinaryExpression::BinaryExpression(const std::shared_ptr<Expression>& left,
                                       token::TokenType operatorType,
                                       std::size_t opIndex,
                                       const std::shared_ptr<Expression>& right) {
        _left = left;
        _right = right;
        _operatorType = operatorType;
        _opIndex = opIndex;
    }

    token::TokenType BinaryExpression::getOperatorType() {
        return _operatorType;
    }

    void BinaryExpression::writeCurrentInfo(std::ostream &s) {
        Expression::writeCurrentInfo(s);
    }

    void BinaryExpression::analyseType() {

    }

    SyntaxType BinaryExpression::getType() {
        return SyntaxType::BinaryExpression;
    }

    bool BinaryExpression::hasChild() {
        return true;
    }

    std::size_t BinaryExpression::start() {
        return _left->start();
    }

    std::size_t BinaryExpression::end() {
        if (_right->getType() != SyntaxType::NullExpression)
            return _right->end();
        return _opIndex;
    }

    std::string BinaryExpression::toString() {
        if (_operatorType == token::TokenType::op_period)
            return _left->toString() + token::TokenUtil::getSpell(_operatorType) + _right->toString();
        return _left->toString() + " " + token::TokenUtil::getSpell(_operatorType) + " " + _right->toString();
    }
}