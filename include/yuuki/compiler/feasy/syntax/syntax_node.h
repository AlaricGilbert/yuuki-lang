#ifndef YUUKI_SYNTAX_NODE_H
#define YUUKI_SYNTAX_NODE_H

#include <vector>
#include <memory>
namespace yuuki::compiler::feasy::syntax{
    class SyntaxNode{
    public:
        virtual void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>)>& syntaxWalker) = 0;
    };
}
#endif //YUUKI_SYNTAX_NODE_H
