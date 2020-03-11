#ifndef YUUKI_GENERIC_H
#define YUUKI_GENERIC_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/type.h>
namespace yuuki::compiler::feasy::syntax {
    class GenericDeclaration : public SyntaxNode, public ISyntaxList<IdentifierName> {
    public:
        explicit GenericDeclaration(std::size_t startLessOp,std::size_t endGreaterOp = invalidTokenIndex);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void add(const std::shared_ptr<IdentifierName> &genericName) override;
        void setEndOpIndex(std::size_t endGreaterOp);
        bool hasChild() override;
        std::size_t start() override;
        std::size_t end() override;
        SyntaxType getType() override;

    private:
        std::vector<std::shared_ptr<IdentifierName>> _genericNames;
        std::size_t _start;
        std::size_t _end;
    };

    class GenericArgumentList : public SyntaxNode, public ISyntaxList<Type> {
    public:
        explicit GenericArgumentList(std::size_t startLessOp,std::size_t endGreaterOp = invalidTokenIndex);
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void add(const std::shared_ptr<Type> &types) override;
        void setEndOpIndex(std::size_t endGreaterOp);
        bool hasChild() override;
        std::size_t start() override;
        std::size_t end() override;
        SyntaxType getType() override;
        std::string toString();

    private:
        std::vector<std::shared_ptr<Type>> _genericTypes;
        std::size_t _start;
        std::size_t _end;
    };
}
#endif //YUUKI_GENERIC_H
