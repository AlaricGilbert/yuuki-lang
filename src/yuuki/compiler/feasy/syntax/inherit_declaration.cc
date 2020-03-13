#include <yuuki/compiler/feasy/syntax/inherit_declaration.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    void InheritDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _inheritInfos.size();id++){
            syntaxWalker(_inheritInfos[id], id == _inheritInfos.size() - 1);
        }
    }

    void InheritDeclaration::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "InheritDeclaration "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "InheritDeclaration "
              << "<" << this << "> " << std::endl;
        }
    }

    void InheritDeclaration::add(const std::shared_ptr<Type> &type) {
        _inheritInfos.push_back(type);
    }

    SyntaxType InheritDeclaration::getType() {
        return SyntaxType::InheritDeclaration;
    }

    bool InheritDeclaration::hasChild() {
        return !_inheritInfos.empty();
    }

    std::size_t InheritDeclaration::start() {
        if(_inheritInfos.empty())
            return invalidTokenIndex;
        return _inheritInfos.front()->start();
    }

    std::size_t InheritDeclaration::end() {
        if(_inheritInfos.empty())
            return invalidTokenIndex;
        return _inheritInfos.back()->end();
    }
}