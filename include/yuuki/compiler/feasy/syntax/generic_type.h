#ifndef YUUKI_GENERIC_TYPE_H
#define YUUKI_GENERIC_TYPE_H

#include <yuuki/compiler/feasy/syntax/type.h>
#include <yuuki/compiler/feasy/syntax/generic.h>
namespace yuuki::compiler::feasy::syntax{
    class GenericType:public Type{
    public:
        GenericType(const std::shared_ptr<Type>& mainType,const std::shared_ptr<GenericArgumentList> &genericInfo);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override;
        std::size_t end() override;
        std::string toString() override ;

    private:
        std::shared_ptr<Type> _mainType;
        std::shared_ptr<GenericArgumentList> _genericInfo;

    };
}
#endif //YUUKI_GENERIC_TYPE_H
