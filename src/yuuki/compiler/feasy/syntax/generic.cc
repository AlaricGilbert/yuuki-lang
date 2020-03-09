#include <yuuki/compiler/feasy/syntax/generic.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    void GenericDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _genericNames.size();id++){
            syntaxWalker(_genericNames[id], id == _genericNames.size() - 1);
        }
    }

    void GenericDeclaration::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "GenericDeclaration "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "GenericDeclaration "
              << "<" << this << "> " << std::endl;
        }
    }

    void GenericDeclaration::add(const std::shared_ptr<IdentifierName> &genericName) {
        _genericNames.push_back(genericName);
    }

    SyntaxType GenericDeclaration::getType() {
        return SyntaxType::GenericDeclaration;
    }

    bool GenericDeclaration::hasChild() {
        return !_genericNames.empty();
    }

    GenericDeclaration::GenericDeclaration(std::size_t startLessOp, std::size_t endGreaterOp) {
        _start = startLessOp;
        _end = endGreaterOp;
    }

    void GenericDeclaration::setEndOpIndex(std::size_t endGreaterOp) {
        _end = endGreaterOp;
    }

    std::size_t GenericDeclaration::start() {
        return _start;
    }

    std::size_t GenericDeclaration::end() {
        if(_end == invalidTokenIndex){
            if(_genericNames.empty())
                return _start;
            return _genericNames.back()->end();
        }
        return _end;
    }

    void GenericArgumentList::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _genericTypes.size();id++){
            syntaxWalker(_genericTypes[id], id == _genericTypes.size() - 1);
        }
    }

    void GenericArgumentList::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "GenericArgumentList "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "GenericArgumentList "
              << "<" << this << "> " << std::endl;
        }
    }

    void GenericArgumentList::add(const std::shared_ptr<Type> &type) {
        _genericTypes.push_back(type);
    }

    bool GenericArgumentList::hasChild() {
        return !_genericTypes.empty();
    }

    SyntaxType GenericArgumentList::getType() {
        return SyntaxType::GenericArgumentList;
    }

    GenericArgumentList::GenericArgumentList(std::size_t startLessOp, std::size_t endGreaterOp) {
        _start = startLessOp;
        _end = endGreaterOp;
    }

    void GenericArgumentList::setEndOpIndex(std::size_t endGreaterOp) {
        _end = endGreaterOp;
    }

    std::size_t GenericArgumentList::start() {
        return _start;
    }

    std::size_t GenericArgumentList::end() {
        if(_end == invalidTokenIndex){
            if(_genericTypes.empty())
                return _start;
            return _genericTypes.back()->end();
        }
        return _end;
    }
}