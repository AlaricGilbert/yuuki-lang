#include <yuuki/compiler/feasy/syntax/name.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    IdentifierName::IdentifierName(const std::string &id, std::size_t tokId) {
        _identifier = id;
        _tokenId = tokId;
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

    bool IdentifierName::hasChild() {
        return false;
    }

    std::size_t IdentifierName::start() {
        return _tokenId;
    }

    std::size_t IdentifierName::end() {
        return _tokenId;
    }

    QualifiedName::QualifiedName(const std::shared_ptr<IdentifierName> &left,
            std::size_t tokID, const std::shared_ptr<Name> &right) {
        _left = left;
        _right = right;
        _periodTokId = tokID;
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

    bool QualifiedName::hasChild() {
        return true;
    }

    std::size_t QualifiedName::start() {
        return _left->start();
    }

    std::size_t QualifiedName::end() {
        return _right->end();
    }

    NameExpression::NameExpression(const std::shared_ptr<Name>& name) {
        _name = name;
    }

    void NameExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, true);
    }

    void NameExpression::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "NameExpression "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "NameExpression "
              << "<" << this << "> " << std::endl;
        }
    }

    SyntaxType NameExpression::getType() {
        return SyntaxType::NameExpression;
    }

    bool NameExpression::hasChild() {
        return true;
    }

    void NameExpression::analyseType() {

    }

    std::size_t NameExpression::start() {
        return _name->start();
    }

    std::size_t NameExpression::end() {
        return _name->end();
    }
}