#include <yuuki/compiler/feasy/syntax/syntax_tree.h>

namespace yuuki::compiler::feasy::syntax{

    void SyntaxTree::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>)> &syntaxWalker) {
        for(auto& nodePtr :_children){
            syntaxWalker(nodePtr);
        }
    }

    void SyntaxTree::add(const std::shared_ptr<SyntaxNode>& child) {
        _children.push_back(child);
    }
}