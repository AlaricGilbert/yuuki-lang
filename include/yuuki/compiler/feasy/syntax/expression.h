#ifndef YUUKI_EXPRESSION_H
#define YUUKI_EXPRESSION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
namespace yuuki::compiler::feasy::syntax{

    class Expression:public SyntaxNode{
    public:
        virtual void analyseType() = 0;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        std::size_t start() override = 0;
        std::size_t end() override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;
    };

}
#endif //YUUKI_EXPRESSION_H
