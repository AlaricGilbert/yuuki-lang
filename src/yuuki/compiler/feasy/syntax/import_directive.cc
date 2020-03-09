#include <yuuki/compiler/feasy/syntax/import_directive.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax {

    void ImportDirective::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_namespaceName, true);
    }

    void ImportDirective::writeCurrentInfo(std::ostream &s) {
        if (s.rdbuf() == std::cout.rdbuf()) {
            s << rang::fg::gray << "ImportDirective "
              << rang::fg::yellow << "<" << this << "> " << std::endl;
        } else {
            s << "ImportDirective "
              << "<" << this << "> " << std::endl;
        }
    }

    SyntaxType ImportDirective::getType() {
        return SyntaxType::ImportDirective;
    }

    bool ImportDirective::hasChild() {
        return true;
    }

    std::size_t ImportDirective::start() {
        return _importTokenIndex == invalidTokenIndex? _namespaceName->start():_importTokenIndex;
    }

    std::size_t ImportDirective::end() {
        return _semiTokenIndex == invalidTokenIndex ?_namespaceName->end():_semiTokenIndex;
    }

    ImportDirective::ImportDirective(const std::shared_ptr<Name> &namespaceName, std::size_t importTokenIndex,
                                     std::size_t semiTokenIndex) {
        _namespaceName = namespaceName;
        _importTokenIndex = importTokenIndex;
        _semiTokenIndex = semiTokenIndex;
    }
}