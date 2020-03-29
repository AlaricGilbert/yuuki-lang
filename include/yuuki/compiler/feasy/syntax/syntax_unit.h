#ifndef YUUKI_SYNTAX_UNIT_H
#define YUUKI_SYNTAX_UNIT_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/import_directive.h>
#include <yuuki/compiler/feasy/syntax/namespace_declaration.h>

namespace yuuki::compiler::feasy::syntax{
    class SyntaxUnit: public SyntaxNode,
                      public ISyntaxList<SyntaxNode>{
    public:
        SyntaxType getType() override;
        bool hasChild() override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void add(const std::shared_ptr<SyntaxNode> &child);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        std::size_t start() override;
        std::size_t end() override;
    private:
        std::vector<std::shared_ptr<SyntaxNode>> _syntax;
    };
}
#endif //YUUKI_SYNTAX_UNIT_H
