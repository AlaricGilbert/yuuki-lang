#ifndef YUUKI_TYPE_H
#define YUUKI_TYPE_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/name.h>
namespace yuuki::compiler::feasy::syntax{
    class Type :public SyntaxNode{
    public:
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;
        virtual std::string toString() = 0;
    };
    class TrivialType:public Type{
    public:
        explicit TrivialType(const std::shared_ptr<Name> &typeName);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::string toString() override ;

    private:
        std::shared_ptr<Name> _typeName;
    };
    class GenericType:public Type{
    public:
        explicit GenericType(const std::shared_ptr<Name> &typeName);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        void addGenericType(const std::shared_ptr<Type> &type);
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::string toString() override ;

    private:
        std::shared_ptr<Name> _typeName;
        std::vector<std::shared_ptr<Type>> _genericTypes;

    };
    class ArrayType:public Type{
    public:
        explicit ArrayType(const std::shared_ptr<Type> &type);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::string toString() override ;

    private:
        std::shared_ptr<Type> _childType;
    };
}
#endif //YUUKI_TYPE_H
