#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    GenericType::GenericType(const std::shared_ptr<Type> &typeName,const std::shared_ptr<GenericTypeList> &genericInfo) {
        _mainType = typeName;
        _genericList = genericInfo;
    }

    GenericType::GenericType(const std::shared_ptr<Type> &typeName){
        _mainType = typeName;
        _genericList = std::make_shared<GenericTypeList>();
    }

    std::shared_ptr<GenericTypeList> GenericType::getGenericList(){
        return _genericList;
    }

    void GenericType::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_mainType, false);
        syntaxWalker(_genericList, true);
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
        return _mainType->toString() + _genericList->toString();
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
