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
        std::size_t start() override = 0;
        std::size_t end() override = 0;
        virtual std::string toString() = 0;
    };

    class TrivialType:public Type{
    public:
        explicit TrivialType(const std::shared_ptr<Name> &typeName);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override;
        std::size_t end() override;
        std::string toString() override ;

    private:
        std::shared_ptr<Name> _typeName;
    };
    class UnknownType:public Type{
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override;
        std::size_t end() override;
        std::string toString() override ;

    };
    class ArrayType:public Type{
    public:
        explicit ArrayType(const std::shared_ptr<Type> &type, std::size_t lSquareIndex);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override;
        std::size_t end() override;
        void setRSquareIndex(std::size_t rSquareIndex);
        std::string toString() override ;

    private:
        std::shared_ptr<Type> _childType;
        std::size_t _lSquareIndex;
        std::size_t _rSquareIndex;
    };
}
#endif //YUUKI_TYPE_H
