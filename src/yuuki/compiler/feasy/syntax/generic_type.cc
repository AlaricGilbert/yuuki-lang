#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    GenericType::GenericType(const std::shared_ptr<Type> &typeName,const std::shared_ptr<GenericArgumentList> &genericInfo) {
        _mainType = typeName;
        _genericInfo = genericInfo;
    }

    void GenericType::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_mainType, false);
        syntaxWalker(_genericInfo, true);
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

    std::string GenericType::toString() {
        return _mainType->toString() + _genericInfo->toString();
    }

    SyntaxType GenericType::getType() {
        return SyntaxType::GenericType;
    }

    bool GenericType::hasChild() {
        return true;
    }

    std::size_t GenericType::start() {
        return 0;
    }

    std::size_t GenericType::end() {
        return 0;
    }
}
