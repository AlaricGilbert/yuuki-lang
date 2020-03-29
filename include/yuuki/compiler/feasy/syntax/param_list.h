#ifndef YUUKI_PARAM_LIST_H
#define YUUKI_PARAM_LIST_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/type.h>
namespace yuuki::compiler::feasy::syntax{
    class ParamDeclaration:public SyntaxNode{
    public:
        ParamDeclaration(const std::shared_ptr<Type>& type, const std::shared_ptr<Name> &name);
        void setDefaultValue(const std::shared_ptr<Expression>& defaultValue);
        void setEqualTokenIndex(std::size_t equalIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::weak_ptr<Name> getParamName();
        std::weak_ptr<Type> getParamType();
        std::weak_ptr<Expression> getDefaultValue();
        std::size_t start() override;
        std::size_t end() override;
        std::size_t getEqualIndex();
    private:
        std::shared_ptr<Name> _name;
        std::shared_ptr<Type> _type;
        std::shared_ptr<Expression> _defaultValue;
        std::size_t _equalIndex;
    };
    class ParamList: public SyntaxNode, public ISyntaxList<ParamDeclaration>{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        void add(const std::shared_ptr<ParamDeclaration>& param) override ;
        std::size_t start() override;
        std::size_t end() override;
    private:
        std::vector<std::shared_ptr<ParamDeclaration>> _params;
    };
}

#endif //YUUKI_PARAM_LIST_H
