#include <yuuki/compiler/feasy/syntax/namespace_declaration.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{
    NamespaceDeclaration::NamespaceDeclaration(const std::shared_ptr<Name> &nsName) {
        _nsName = nsName;
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
}