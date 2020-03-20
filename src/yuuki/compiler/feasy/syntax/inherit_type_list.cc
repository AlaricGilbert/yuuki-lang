#include <yuuki/compiler/feasy/syntax/inherit_type_list.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    void InheritTypeList::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _inheritInfos.size();id++){
            syntaxWalker(_inheritInfos[id], id == _inheritInfos.size() - 1);
        }
    }

    void InheritTypeList::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "InheritDeclaration "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "InheritDeclaration "
              << "<" << this << "> " << std::endl;
        }
    }

    void InheritTypeList::add(const std::shared_ptr<Type> &type) {
        _inheritInfos.push_back(type);
    }

    SyntaxType InheritTypeList::getType() {
        return SyntaxType::InheritDeclaration;
    }

    bool InheritTypeList::hasChild() {
        return !_inheritInfos.empty();
    }

    std::size_t InheritTypeList::start() {
        if(_inheritInfos.empty())
            return invalidTokenIndex;
        return _inheritInfos.front()->start();
    }

    std::size_t InheritTypeList::end() {
        if(_inheritInfos.empty())
            return invalidTokenIndex;
        return _inheritInfos.back()->end();
    }
}