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

    void InheritDeclaration::addInheritType(const std::shared_ptr<Type> &type) {
        _inheritInfos.push_back(type);
    }

    SyntaxType InheritDeclaration::getType() {
        return SyntaxType::InheritDeclaration;
    }
}