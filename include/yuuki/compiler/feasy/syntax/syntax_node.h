#ifndef YUUKI_SYNTAX_NODE_H
#define YUUKI_SYNTAX_NODE_H

#include <functional>
#include <vector>
#include <iostream>
#include <memory>
namespace yuuki::compiler::feasy::syntax{
    class SyntaxNode{
    public:
        virtual void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) = 0;
        virtual void writeCurrentInfo(std::ostream& ostream) = 0;
        virtual ~SyntaxNode() = default;
    };
}
#endif //YUUKI_SYNTAX_NODE_H
