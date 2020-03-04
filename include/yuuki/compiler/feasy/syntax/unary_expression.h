#ifndef YUUKI_UNARY_EXPRESSION_H
#define YUUKI_UNARY_EXPRESSION_H
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::syntax{
    class UnaryExpression: public Expression{
    public:
        UnaryExpression(token::TokenType operatorType, const std::shared_ptr<Expression>& right);
        token::TokenType getOperatorType();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void analyseType() override;
    private:
        token::TokenType _operatorType;
        std::shared_ptr<Expression> _right;
    };
}
#endif //YUUKI_UNARY_EXPRESSION_H
