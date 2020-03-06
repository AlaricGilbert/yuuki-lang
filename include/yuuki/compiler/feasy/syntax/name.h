#ifndef YUUKI_NAME_H
#define YUUKI_NAME_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/expression.h>
#include <string>

namespace yuuki::compiler::feasy::syntax{
    class Name: public SyntaxNode{
    public:
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;
        virtual std::string toString() = 0;
    };

    class IdentifierName: public Name{
    public:
        explicit IdentifierName(const std::string& id);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::string toString() override;
    private:
        std::string _identifier;
    };
    class QualifiedName: public Name{
    public:
        QualifiedName(const std::shared_ptr<Name>& left,
                      const std::shared_ptr<IdentifierName>& right);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::string toString() override;
    private:
        std::shared_ptr<Name> _left;
        std::shared_ptr<IdentifierName> _right;
    };

    class NameExpression: public Expression{
    public:
        explicit NameExpression(const std::shared_ptr<Name>& name);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        void analyseType() override ;

    private:
        std::shared_ptr<Name> _name;
    };
}
#endif //YUUKI_NAME_H
