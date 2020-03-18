#ifndef YUUKI_UNARY_EXPRESSION_H
#define YUUKI_UNARY_EXPRESSION_H
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::syntax{
    class UnaryExpression: public Expression{
    public:
        UnaryExpression(token::TokenType operatorType, std::size_t opIndex, const std::shared_ptr<Expression>& right);
        token::TokenType getOperatorType();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        SyntaxType getType() override;
        std::size_t start() override;
        std::size_t end() override;
        bool hasChild() override;
        void analyseType() override;
        std::string toString() override;
    private:
        token::TokenType _operatorType;
        std::size_t _opIndex;
        std::shared_ptr<Expression> _right;
    };
}
#endif //YUUKI_UNARY_EXPRESSION_H
