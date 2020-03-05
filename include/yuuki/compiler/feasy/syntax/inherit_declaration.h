#ifndef YUUKI_INHERIT_DECLARATION_H
#define YUUKI_INHERIT_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/type.h>
namespace yuuki::compiler::feasy::syntax{
    class InheritDeclaration:public SyntaxNode{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        void addInheritType(const std::shared_ptr<Type> &type);
        SyntaxType getType() override ;

    private:
        std::vector<std::shared_ptr<Type>> _inheritInfos;
    };
}
#endif //YUUKI_INHERIT_DECLARATION_H
