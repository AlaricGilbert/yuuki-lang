#include <yuuki/compiler/feasy/syntax/name.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    IdentifierName::IdentifierName(const std::string &id) {
        _identifier = id;
    }

    std::string IdentifierName::toString(){
        return _identifier;
    }

    void IdentifierName::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {}

    void IdentifierName::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray    << "IdentifierName "
              << rang::fg::yellow  << "<"  << this         << "> "
              << rang::fg::green   << "'"  << _identifier  << "'"
              << rang::fg::reset << std::endl;
        } else{
            s << "IdentifierName "
              << "<" << this         << "> "
              << "'" << _identifier  << "'" << std::endl;
        }
    }

    SyntaxType IdentifierName::getType() {
        return SyntaxType::IdentifierName;
    }


    QualifiedName::QualifiedName(const std::shared_ptr<Name> &left, const std::shared_ptr<IdentifierName> &right) {
        _left = left;
        _right = right;
    }

    void QualifiedName::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_left, false);
        syntaxWalker(_right, true);
    }

    void QualifiedName::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray    << "QualifiedName "
              << rang::fg::yellow  << "<" << this << "> "
              << rang::fg::green   << "'"  << toString()  << "'"
              << rang::fg::reset << std::endl;
        } else{
            s << "QualifiedName "
              << "<" << this << "> "
              << "'"  << toString()  << "'" << std::endl;
        }
    }

    std::string QualifiedName::toString() {
        return _left->toString() + '.' + _right->toString();
    }

    SyntaxType QualifiedName::getType() {
        return SyntaxType::QualifiedName;
    }

}