#include <yuuki/compiler/feasy/syntax/type.h>
#include <rang/rang.h>
#include <sstream>

namespace yuuki::compiler::feasy::syntax{
    TrivialType::TrivialType(const std::shared_ptr<Name> &typeName) {
        _typeName = typeName;
    }

    void TrivialType::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_typeName,true);
    }

    void TrivialType::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray    << "TrivialType "
              << rang::fg::yellow  << "<" << this << "> "
              << rang::fg::green   << "'"  << toString()  << "'"
              << rang::fg::reset << std::endl;
        } else{
            s << "TrivialType "
              << "<" << this << "> "
              << "'"  << toString()  << "'" << std::endl;
        }
    }

    std::string TrivialType::toString() {
        return _typeName->toString();
    }

    SyntaxType TrivialType::getType() {
        return SyntaxType::TrivialType;
    }

    bool TrivialType::hasChild() {
        return false;
    }

    std::size_t TrivialType::start() {
        return _typeName->start();
    }

    std::size_t TrivialType::end() {
        return _typeName->end();
    }

    ArrayType::ArrayType(const std::shared_ptr<Type> &type, std::size_t lSquareIndex) {
        _childType = type;
        _lSquareIndex = lSquareIndex;
        _rSquareIndex = invalidTokenIndex;
    }

    void ArrayType::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_childType,true);
    }

    void ArrayType::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray    << "ArrayType "
              << rang::fg::yellow  << "<" << this << "> "
              << rang::fg::green   << "'"  << toString()  << "'"
              << rang::fg::reset << std::endl;
        } else{
            s << "ArrayType "
              << "<" << this << "> "
              << "'"  << toString()  << "'" << std::endl;
        }
    }

    std::string ArrayType::toString() {
        return _childType->toString() + "[]";
    }

    SyntaxType ArrayType::getType() {
        return SyntaxType::ArrayType;
    }

    bool ArrayType::hasChild() {
        return true;
    }

    std::size_t ArrayType::start() {
        return _childType->start();
    }

    std::size_t ArrayType::end() {
        if(_rSquareIndex != invalidTokenIndex)
            return _rSquareIndex;
        return _lSquareIndex;
    }

    void ArrayType::setRSquareIndex(std::size_t rSquareIndex) {
        _rSquareIndex = rSquareIndex;
    }

    std::shared_ptr<Type> ArrayType::getChildType() {
        return _childType;
    }

    std::size_t ArrayType::getLSquareIndex() {
        return _lSquareIndex;
    }

    std::size_t ArrayType::getRSquareIndex() {
        return _rSquareIndex;
    }

    void UnknownType::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void UnknownType::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::red     << "UnknownType "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "UnknownType "
              << "<" << this << "> " << std::endl;
        }
    }

    SyntaxType UnknownType::getType() {
        return SyntaxType::UnknownType;
    }

    bool UnknownType::hasChild() {
        return false;
    }

    std::size_t UnknownType::start() {
        return invalidTokenIndex;
    }

    std::size_t UnknownType::end() {
        return invalidTokenIndex;
    }

    std::string UnknownType::toString() {
        return "unknown";
    }
}