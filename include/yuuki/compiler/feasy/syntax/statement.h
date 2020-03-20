#ifndef YUUKI_STATEMENT_H
#define YUUKI_STATEMENT_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
namespace yuuki::compiler::feasy::syntax{
    class Statement:public SyntaxNode{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;

    };


}
#endif //YUUKI_STATEMENT_H
