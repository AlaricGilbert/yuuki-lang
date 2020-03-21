#ifndef YUUKI_VARIABLE_DECLARATION_LIST_H
#define YUUKI_VARIABLE_DECLARATION_LIST_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/syntax/type.h>

namespace yuuki::compiler::feasy::syntax{
    class VariableDeclarationList: public SyntaxNode, public ISyntaxList<Expression>{
    public:
        explicit VariableDeclarationList(const std::shared_ptr<Type>& type);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        void add(const std::shared_ptr<Expression>& varDecl);
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::shared_ptr<Type> _type;
        std::vector<std::shared_ptr<Expression>> _varDecls;
    };
}
#endif //YUUKI_VARIABLE_DECLARATION_LIST_H