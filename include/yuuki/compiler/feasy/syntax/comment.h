#ifndef YUUKI_COMMENT_H
#define YUUKI_COMMENT_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
namespace yuuki::compiler::feasy::syntax{
    class InlineComment:public SyntaxNode{
        SyntaxType getType() override;
        bool hasChild() override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
    };

    class InterlineComment:public SyntaxNode{
        SyntaxType getType() override;
        bool hasChild() override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
    };
}
#endif //YUUKI_COMMENT_H
