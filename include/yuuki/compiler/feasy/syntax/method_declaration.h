#ifndef YUUKI_METHOD_DECLARATION_H
#define YUUKI_METHOD_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/statement.h>
#include <yuuki/compiler/feasy/syntax/modifier_list.h>
#include <yuuki/compiler/feasy/syntax/generic.h>
#include <yuuki/compiler/feasy/syntax/param_list.h>
namespace yuuki::compiler::feasy::syntax {
    class MethodDeclaration: public SyntaxNode {
    public:
        MethodDeclaration(const std::shared_ptr<ModifierList>& mod,
                          const std::shared_ptr<Type>& returnType,
                          const std::shared_ptr<Name>& name,
                          const std::shared_ptr<GenericTypeList>& genericInfos,
                          const std::shared_ptr<ParamList>& params,
                          const std::shared_ptr<Statement>& body);
        void setLParenIndex(std::size_t lParenIndex);
        void setRParenIndex(std::size_t rParenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        SyntaxType getType() override ;
        std::size_t start() override;
        std::size_t end() override;
    private:
        std::shared_ptr<ModifierList> _mod;
        std::shared_ptr<Type> _returnType;
        std::shared_ptr<Name> _name;
        std::shared_ptr<GenericTypeList> _genericInfos;
        std::shared_ptr<ParamList> _params;
        std::shared_ptr<Statement> _body;
        std::size_t _lParenIndex;
        std::size_t _rParenIndex;
    };
}
#endif //YUUKI_METHOD_DECLARATION_H
