#ifndef YUUKI_FIELD_DECLARATION_H
#define YUUKI_FIELD_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/variable_declaration_list.h>
#include <yuuki/compiler/feasy/syntax/modifier_list.h>

namespace yuuki::compiler::feasy::syntax{
    class FieldDeclaration:public SyntaxNode{
    public:
        explicit FieldDeclaration(const std::shared_ptr<ModifierList>& modifiers,const std::shared_ptr<Type>& type);
        void setSemiTokenIndex(std::size_t semiTokenIndex);
        std::shared_ptr<VariableDeclarationList> getVarDeclList();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        SyntaxType getType() override ;
        std::size_t start() override;
        std::size_t end() override;

    private:
        std::shared_ptr<ModifierList> _modifiers;
        std::shared_ptr<VariableDeclarationList> _varDeclList;
        std::size_t _semiTokenIndex;
    };
}
#endif //YUUKI_FIELD_DECLARATION_H
