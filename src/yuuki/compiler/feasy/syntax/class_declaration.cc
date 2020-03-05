#include <yuuki/compiler/feasy/syntax/class_declaration.h>
#include <rang/rang.h>
#include <sstream>
#include <memory>

namespace yuuki::compiler::feasy::syntax{

    ClassDeclaration::ClassDeclaration(const std::shared_ptr<ModifierBag> &mod,
                                       const std::shared_ptr<Name> &name,
                                       const std::shared_ptr<InheritDeclaration> &inheritInfos,
                                       const std::shared_ptr<GenericDeclaration> &genericInfos) {
        _mod = mod;
        _name = name;
        _inheritInfos = inheritInfos;
        _genericInfos = genericInfos;
    }

    void ClassDeclaration::add(const std::shared_ptr<SyntaxNode> &member) {
        _members.push_back(member);
    }

    void ClassDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, false);
        if(_mod->hasChild())
            syntaxWalker(_mod, false);
        if(_inheritInfos->hasChild())
            syntaxWalker(_inheritInfos, false);
        if(_genericInfos->hasChild())
            syntaxWalker(_genericInfos, _members.empty());
        for (int i = 0; i < _members.size(); ++i) {
            syntaxWalker(_members[i], i == _members.size() - 1);
        }
    }

    void ClassDeclaration::writeCurrentInfo(std::ostream &s) {
        std::ostringstream classData;
        if(_mod->hasChild()){
            _mod->forEachChild([&classData](const std::weak_ptr<SyntaxNode> &n, bool isLast) {
                classData << std::static_pointer_cast<ModifierMark>(n.lock())->toString() << " ";
            });
        }
        classData << "'class "
                  << _name->toString();
        if(_genericInfos->hasChild()) {
            classData << "<";
            _genericInfos->forEachChild([&classData](const std::weak_ptr<SyntaxNode> &n, bool isLast) {
                classData << std::static_pointer_cast<GenericType>(n.lock())->toString()
                          << (isLast ? ">" : ",");
            });
        }
        if(_inheritInfos->hasChild()) {
            classData << ": ";
            _inheritInfos->forEachChild([&classData](const std::weak_ptr<SyntaxNode> &n, bool isLast) {
                classData << std::static_pointer_cast<Type>(n.lock())->toString()
                          << (isLast ? "'" : ", ");
            });
        }
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "ClassDeclaration "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::green    << classData.str()
              << rang::fg::reset    << std::endl;
        } else{
            s << "ClassDeclaration "
              << "<" << this << "> "
              << classData.str() << std::endl;
        }
    }

    SyntaxType ClassDeclaration::getType() {
        return SyntaxType::ClassDeclaration;
    }

    bool ClassDeclaration::hasChild() {
        return true;
    }
}