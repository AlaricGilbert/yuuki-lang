#ifndef YUUKI_SYNTAX_TREE_H
#define YUUKI_SYNTAX_TREE_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <vector>
namespace yuuki::compiler::feasy::syntax{
    class SyntaxTree: public SyntaxNode{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;
        virtual void add(const std::shared_ptr<SyntaxNode>& child) = 0;

    };
}
#endif //YUUKI_SYNTAX_TREE_H
