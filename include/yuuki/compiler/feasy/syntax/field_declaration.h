#ifndef YUUKI_FIELD_DECLARATION_H
#define YUUKI_FIELD_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/statement.h>
#include <yuuki/compiler/feasy/syntax/modifier_list.h>

namespace yuuki::compiler::feasy::syntax{
    class FieldDeclaration:public SyntaxNode{
    public:
        explicit FieldDeclaration(const std::shared_ptr<ModifierList>& modifiers,
                const std::shared_ptr<VariableDeclarationStatement>& varDecl);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        SyntaxType getType() override ;
        std::size_t start() override;
        std::size_t end() override;

    private:
        std::shared_ptr<ModifierList> _modifiers;
        std::shared_ptr<VariableDeclarationStatement> _varDecl;
    };
}
#endif //YUUKI_FIELD_DECLARATION_H
