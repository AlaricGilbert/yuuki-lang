#ifndef YUUKI_NAMESPACE_DECLARATION_H
#define YUUKI_NAMESPACE_DECLARATION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/class_declaration.h>
namespace yuuki::compiler::feasy::syntax{
    class NamespaceDeclaration: public SyntaxNode,
                                public ISyntaxList<ClassDeclaration>{
    public:
        explicit NamespaceDeclaration(const std::shared_ptr<Name>& nsName);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        bool hasChild() override;
        SyntaxType getType() override;
        void add(const std::shared_ptr<ClassDeclaration> &child) override;
    private:
        std::shared_ptr<Name> _nsName;
        std::vector<std::shared_ptr<SyntaxNode>> _children;
    };


}
#endif //YUUKI_NAMESPACE_DECLARATION_H
