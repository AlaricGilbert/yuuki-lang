#ifndef YUUKI_CLASS_DECLARATION_H
#define YUUKI_CLASS_DECLARATION_H
#include <yuuki/compiler/feasy/syntax/syntax_tree.h>
#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <yuuki/compiler/feasy/syntax/import_directive.h>
#include <yuuki/compiler/feasy/syntax/inherit_declaration.h>
#include <yuuki/compiler/feasy/syntax/generic_declaration.h>
#include <yuuki/compiler/feasy/syntax/modifier_bag.h>
#include <yuuki/compiler/feasy/syntax/type.h>
#include <string>
#include <memory>
namespace yuuki::compiler::feasy::syntax{
    class FieldDeclaration:public SyntaxTree{

    };
    class ClassDeclaration:public SyntaxTree{
    public:
        explicit ClassDeclaration(const std::shared_ptr<ModifierBag> &mod,
                                  const std::shared_ptr<Name> &name,
                                  const std::shared_ptr<InheritDeclaration>& inheritInfos,
                                  const std::shared_ptr<GenericDeclaration>& genericInfos);
        void add(const std::shared_ptr<SyntaxNode> &member) override ;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        SyntaxType getType() override ;
    private:
        std::shared_ptr<ModifierBag> _mod;
        std::shared_ptr<Name> _name;
        std::shared_ptr<InheritDeclaration> _inheritInfos;
        std::shared_ptr<GenericDeclaration> _genericInfos;
        std::vector<std::shared_ptr<SyntaxNode>> _members;
    };
}
#endif //YUUKI_CLASS_DECLARATION_H
