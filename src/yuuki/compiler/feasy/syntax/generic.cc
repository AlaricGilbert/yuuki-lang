#include <yuuki/compiler/feasy/syntax/generic.h>
#include <rang/rang.h>
#include <sstream>

namespace yuuki::compiler::feasy::syntax{
    void GenericTypeList::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _genericTypes.size();id++){
            syntaxWalker(_genericTypes[id], id == _genericTypes.size() - 1);
        }
    }

    void GenericTypeList::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "GenericArgumentList "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "GenericArgumentList "
              << "<" << this << "> " << std::endl;
        }
    }

    void GenericTypeList::add(const std::shared_ptr<Type> &type) {
        _genericTypes.push_back(type);
    }

    bool GenericTypeList::hasChild() {
        return !_genericTypes.empty();
    }

    SyntaxType GenericTypeList::getType() {
        return SyntaxType::GenericArgumentList;
    }

    GenericTypeList::GenericTypeList(std::size_t startLessOp, std::size_t endGreaterOp) {
        _start = startLessOp;
        _end = endGreaterOp;
    }

    void GenericTypeList::setStartOpIndex(std::size_t startLessOp){
        _start = startLessOp;
    }

    void GenericTypeList::setEndOpIndex(std::size_t endGreaterOp) {
        _end = endGreaterOp;
    }

    std::size_t GenericTypeList::start() {
        return _start;
    }

    std::size_t GenericTypeList::end() {
        if(_end == invalidTokenIndex){
            if(_genericTypes.empty())
                return _start;
            return _genericTypes.back()->end();
        }
        return _end;
    }

    std::string GenericTypeList::toString() {
        std::stringstream result;
        result << "<";
        for (std::size_t i = 0; i < _genericTypes.size(); ++i) {
            result << _genericTypes[i]->toString();
            result << ((i == _genericTypes.size() - 1)? ">" :"," );
        }
        return result.str();
    }
}