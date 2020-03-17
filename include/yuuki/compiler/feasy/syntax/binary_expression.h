#ifndef YUUKI_BINARY_EXPRESSION_H
#define YUUKI_BINARY_EXPRESSION_H
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::syntax{
    class BinaryExpression: public Expression {
    public:
        BinaryExpression(const std::shared_ptr<Expression>& left,
                         token::TokenType operatorType,
                         std::size_t opIndex,
                         const std::shared_ptr<Expression>& right);
        token::TokenType getOperatorType();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>,bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        std::size_t start() override;
        std::size_t end() override;
        bool hasChild() override ;
        void analyseType() override;

    private:
        std::shared_ptr<SyntaxNode> _left;
        token::TokenType _operatorType;
        std::size_t _opIndex;
        std::shared_ptr<SyntaxNode> _right;
    };
}
#endif //YUUKI_BINARY_EXPRESSION_H
