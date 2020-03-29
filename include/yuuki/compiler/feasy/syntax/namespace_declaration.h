#ifndef YUUKI_NAMESPACE_DECLARATION_H
#define YUUKI_NAMESPACE_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/class_declaration.h>
namespace yuuki::compiler::feasy::syntax{
    class NamespaceDeclaration: public SyntaxNode,
                                public ISyntaxList<ClassDeclaration>{
    public:
        explicit NamespaceDeclaration(std::size_t nsTokenIndex,const std::shared_ptr<Name>& nsName);
        void setLBraceTokenIndex(std::size_t lBraceTokenIndex);
        void setRBraceTokenIndex(std::size_t rBraceTokenIndex);
        void forEachClass(const std::function<void (std::weak_ptr<ClassDeclaration>, bool)> &syntaxWalker);
        std::shared_ptr<Name> getNamespaceName();
        std::size_t getLBraceTokenIndex();
        std::size_t getRBraceTokenIndex();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        bool hasChild() override;
        SyntaxType getType() override;
        void add(const std::shared_ptr<ClassDeclaration> &child) override;
        std::size_t start() override;
        std::size_t end() override;

    private:
        std::shared_ptr<Name> _nsName;
        std::vector<std::shared_ptr<ClassDeclaration>> _children;
        std::size_t _nsTokenIndex;
        std::size_t _lBraceTokenIndex;
        std::size_t _rBraceTokenIndex;

    };


}
#endif //YUUKI_NAMESPACE_DECLARATION_H
