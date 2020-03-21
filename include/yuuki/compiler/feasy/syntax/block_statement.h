#ifndef YUUKI_BLOCK_STATEMENT_H
#define YUUKI_BLOCK_STATEMENT_H

#include <yuuki/compiler/feasy/syntax/statement.h>
#include <yuuki/compiler/feasy/syntax/expression.h>
namespace yuuki::compiler::feasy::syntax{
    class BlockStatement: public ISyntaxList<Statement>,public ISyntaxList<Expression>, public Statement{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        void add(const std::shared_ptr<Statement>& child) override ;
        void add(const std::shared_ptr<Expression>& child) override ;

    private:
        std::vector<std::shared_ptr<SyntaxNode>> _children;
    };

}
#endif //YUUKI_BLOCK_STATEMENT_H
