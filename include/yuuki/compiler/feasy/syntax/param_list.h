#ifndef YUUKI_PARAM_LIST_H
#define YUUKI_PARAM_LIST_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/type.h>
namespace yuuki::compiler::feasy::syntax{
    class ParamDeclaration:public SyntaxNode{
    public:
        ParamDeclaration(const std::shared_ptr<Name>& name, const std::shared_ptr<Type> &type);
        void setDefaultValue(const std::shared_ptr<Expression>& defaultValue);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::weak_ptr<Name> getParamName();
        std::weak_ptr<Type> getParamType();


    private:
        std::shared_ptr<Name> _name;
        std::shared_ptr<Type> _type;
        std::shared_ptr<Expression> _defaultValue;
    };
    class ParamList: public SyntaxNode, public ISyntaxList<ParamDeclaration>{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        void add(const std::shared_ptr<ParamDeclaration>& param) override ;

    private:
        std::vector<std::shared_ptr<ParamDeclaration>> _params;
    };
}

#endif //YUUKI_PARAM_LIST_H
