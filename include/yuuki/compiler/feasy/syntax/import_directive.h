#ifndef YUUKI_IMPORT_DIRECTIVE_H
#define YUUKI_IMPORT_DIRECTIVE_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/name.h>

namespace yuuki::compiler::feasy::syntax {
    class ImportDirective:public SyntaxNode{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        explicit ImportDirective(const std::shared_ptr<Name> &namespaceName);
    private:
        std::shared_ptr<Name> _namespaceName;
    };
}
#endif //YUUKI_IMPORT_DIRECTIVE_H
