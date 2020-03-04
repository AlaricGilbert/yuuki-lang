#ifndef YUUKI_BINARY_EXPRESSION_H
#define YUUKI_BINARY_EXPRESSION_H
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::syntax{
    class BinaryExpression: public Expression{
    public:
        BinaryExpression(const std::shared_ptr<Expression>& left,
                token::TokenType operatorType, const std::shared_ptr<Expression>& right);
        token::TokenType getOperatorType();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>)> &syntaxWalker) override;
    private:
        std::shared_ptr<Expression> _left;
        token::TokenType _operatorType;
        std::shared_ptr<Expression> _right;
    };
}
#endif //YUUKI_BINARY_EXPRESSION_H
