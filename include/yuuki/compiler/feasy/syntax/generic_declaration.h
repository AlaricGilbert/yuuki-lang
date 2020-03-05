#ifndef YUUKI_GENERIC_DECLARATION_H
#define YUUKI_GENERIC_DECLARATION_H
#include <yuuki/compiler/feasy/syntax/type.h>
namespace yuuki::compiler::feasy::syntax{
    class GenericDeclaration:public SyntaxNode{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        void addGenericType(const std::shared_ptr<Name> &genericName);
        SyntaxType getType() override ;

    private:
        std::vector<std::shared_ptr<Name>> _genericNames;
    };
}
#endif //YUUKI_GENERIC_DECLARATION_H
