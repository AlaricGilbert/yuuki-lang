#ifndef YUUKI_IMPORT_DIRECTIVE_H
#define YUUKI_IMPORT_DIRECTIVE_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/name.h>

namespace yuuki::compiler::feasy::syntax {
    class ImportDirective : public SyntaxNode {
    public:
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;

        void writeCurrentInfo(std::ostream &ostream) override;

        SyntaxType getType() override;

        bool hasChild() override;

        std::size_t start() override;

        std::size_t end() override;

        explicit ImportDirective(const std::shared_ptr<Name> &namespaceName,
                                 std::size_t importTokenIndex = invalidTokenIndex,
                                 std::size_t semiTokenIndex = invalidTokenIndex);

    private:
        std::shared_ptr<Name> _namespaceName;
        std::size_t _importTokenIndex;
        std::size_t _semiTokenIndex;
    };
}
#endif //YUUKI_IMPORT_DIRECTIVE_H
