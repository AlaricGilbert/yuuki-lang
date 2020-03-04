#include <yuuki/compiler/feasy/syntax/binary_expression.h>

#include <utility>

namespace yuuki::compiler::feasy::syntax{
    void BinaryExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>)> &syntaxWalker) {
        syntaxWalker(_left);
        syntaxWalker(_right);
    }

    BinaryExpression::BinaryExpression(const std::shared_ptr<Expression>& left, token::TokenType operatorType,
                                       const std::shared_ptr<Expression>& right) {
        _left = left;
        _right = right;
        _operatorType = operatorType;
    }

    token::TokenType BinaryExpression::getOperatorType() {
        return _operatorType;
    }
}