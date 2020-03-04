#include <yuuki/compiler/feasy/syntax/unary_expression.h>

#include <utility>

namespace yuuki::compiler::feasy::syntax{
    void UnaryExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>)> &syntaxWalker) {
        syntaxWalker(_right);
    }

    UnaryExpression::UnaryExpression(token::TokenType operatorType,
                                     const std::shared_ptr<Expression>& right) {
        _right = right;
        _operatorType = operatorType;
    }

    token::TokenType UnaryExpression::getOperatorType() {
        return _operatorType;
    }
}