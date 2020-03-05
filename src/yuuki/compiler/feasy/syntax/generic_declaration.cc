#include <yuuki/compiler/feasy/syntax/generic_declaration.h>
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

    void GenericDeclaration::addGenericType(const std::shared_ptr<Name> &genericName) {
        _genericNames.push_back(genericName);
    }

    SyntaxType GenericDeclaration::getType() {
        return SyntaxType::GenericDeclaration;
    }

    bool GenericDeclaration::hasChild() {
        return !_genericNames.empty();
    }
}