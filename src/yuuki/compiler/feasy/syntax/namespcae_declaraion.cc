#include <yuuki/compiler/feasy/syntax/namespace_declaration.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{
    NamespaceDeclaration::NamespaceDeclaration(std::size_t nsTokenIndex, const std::shared_ptr<Name> &nsName) {
        _nsName = nsName;
        _nsTokenIndex = nsTokenIndex;
        _lBraceTokenIndex = invalidTokenIndex;
        _rBraceTokenIndex = invalidTokenIndex;
    }

    void NamespaceDeclaration::setLBraceTokenIndex(std::size_t lBraceTokenIndex) {
        _lBraceTokenIndex = lBraceTokenIndex;
    }

    void NamespaceDeclaration::setRBraceTokenIndex(std::size_t rBraceTokenIndex) {
        _rBraceTokenIndex = rBraceTokenIndex;
    }

    void NamespaceDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_nsName,_children.empty());
        for (std::size_t i = 0; i < _children.size(); ++i) {
            syntaxWalker(_children[i], i == _children.size() - 1);
        }
    }

    void NamespaceDeclaration::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "NamespaceDeclaration "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::green    << "'namespace " << _nsName->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "NamespaceDeclaration "
              << "<" << this << "> "
              << "'namespace " << _nsName->toString()
              << "'"<< std::endl;
        }
    }

    bool NamespaceDeclaration::hasChild() {
        return true;
    }

    SyntaxType NamespaceDeclaration::getType() {
        return SyntaxType::NamespaceDeclaration;
    }

    void NamespaceDeclaration::add(const std::shared_ptr<ClassDeclaration> &child) {
        _children.push_back(child);
    }

    std::size_t NamespaceDeclaration::start() {
        return _nsTokenIndex;
    }

    std::size_t NamespaceDeclaration::end() {
        return 0;
    }

    std::shared_ptr<Name> NamespaceDeclaration::getNamespaceName() {
        return _nsName;
    }

    std::size_t NamespaceDeclaration::getLBraceTokenIndex() {
        return _lBraceTokenIndex;
    }

    std::size_t NamespaceDeclaration::getRBraceTokenIndex() {
        return _rBraceTokenIndex;
    }


    void
    NamespaceDeclaration::forEachClass(const std::function<void(std::weak_ptr<ClassDeclaration>, bool)> &syntaxWalker) {
        for (std::size_t i = 0; i < _children.size(); ++i) {
            syntaxWalker(_children[i], i == _children.size() - 1);
        }
    }
}