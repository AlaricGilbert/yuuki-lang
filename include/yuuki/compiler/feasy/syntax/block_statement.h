#ifndef YUUKI_BLOCK_STATEMENT_H
#define YUUKI_BLOCK_STATEMENT_H

#include <yuuki/compiler/feasy/syntax/statement.h>
#include <yuuki/compiler/feasy/syntax/syntax_tree.h>
#include <vector>
namespace yuuki::compiler::feasy::syntax{
    class BlockStatement: public SyntaxTree, public Statement{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        void add(const std::shared_ptr<SyntaxNode>& child) override ;

    private:
        std::vector<std::shared_ptr<SyntaxNode>> _children;
    };

}
#endif //YUUKI_BLOCK_STATEMENT_H
