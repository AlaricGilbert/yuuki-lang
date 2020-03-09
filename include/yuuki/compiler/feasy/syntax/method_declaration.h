#ifndef YUUKI_METHOD_DECLARATION_H
#define YUUKI_METHOD_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <yuuki/compiler/feasy/syntax/modifier_list.h>
#include <yuuki/compiler/feasy/syntax/generic.h>
#include <yuuki/compiler/feasy/syntax/param_list.h>
namespace yuuki::compiler::feasy::syntax {
    class MethodDeclaration: public SyntaxNode {
    public:
        MethodDeclaration(const std::shared_ptr<ModifierList>& mod,
                          const std::shared_ptr<Type>& returnType,
                          const std::shared_ptr<Name>& name,
                          const std::shared_ptr<GenericDeclaration>& genericInfos,
                          const std::shared_ptr<ParamList>& params,
                          const std::shared_ptr<BlockStatement>& body);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        SyntaxType getType() override ;
    private:
        std::shared_ptr<ModifierList> _mod;
        std::shared_ptr<Type> _returnType;
        std::shared_ptr<Name> _name;
        std::shared_ptr<GenericDeclaration> _genericInfos;
        std::shared_ptr<ParamList> _params;
        std::shared_ptr<BlockStatement> _body;
    };
}
#endif //YUUKI_METHOD_DECLARATION_H
