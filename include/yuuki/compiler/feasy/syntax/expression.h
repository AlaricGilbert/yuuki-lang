#ifndef YUUKI_EXPRESSION_H
#define YUUKI_EXPRESSION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
namespace yuuki::compiler::feasy::syntax{
    class Expression:public SyntaxNode{
    public:
        virtual void analyseType() = 0;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>)> &syntaxWalker) override = 0;
    };
}
#endif //YUUKI_EXPRESSION_H
