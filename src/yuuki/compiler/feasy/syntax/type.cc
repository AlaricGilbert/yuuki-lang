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

    GenericType::GenericType(const std::shared_ptr<Name> &typeName) {
        _typeName = typeName;
    }

    void GenericType::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_typeName, false);
        for(size_t id = 0; id < _genericTypes.size();id++){
            syntaxWalker(_genericTypes[id], id == _genericTypes.size() - 1);
        }
    }

    void GenericType::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray    << "GenericType "
              << rang::fg::yellow  << "<" << this << "> "
              << rang::fg::green   << "'"  << toString()  << "'"
              << rang::fg::reset << std::endl;
        } else{
            s << "GenericType "
              << "<" << this << "> "
              << "'"  << toString()  << "'" << std::endl;
        }
    }

    void GenericType::addGenericType(const std::shared_ptr<Type> &type) {
        _genericTypes.push_back(type);
    }

    std::string GenericType::toString() {
        std::ostringstream result;
        result << _typeName->toString() << "<";
        for(size_t id = 0; id < _genericTypes.size() - 1;id++){
            result << _genericTypes[id]->toString() << ",";
        }
        result << _genericTypes.back()->toString() <<"> ";
        return result.str();
    }

    SyntaxType GenericType::getType() {
        return SyntaxType::GenericType;
    }

    ArrayType::ArrayType(const std::shared_ptr<Type> &type) {
        _childType = type;
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
}