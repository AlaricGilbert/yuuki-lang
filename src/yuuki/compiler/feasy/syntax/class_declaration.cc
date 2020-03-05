#include <yuuki/compiler/feasy/syntax/class_declaration.h>
#include <rang/rang.h>
#include <sstream>
#include <memory>

namespace yuuki::compiler::feasy::syntax{

    ClassDeclaration::ClassDeclaration(const std::shared_ptr<Name> &name,
                                       const std::shared_ptr<InheritDeclaration> &inheritInfos,
                                       const std::shared_ptr<GenericDeclaration> &genericInfos) {
        _name = name;
        _inheritInfos = inheritInfos;
        _genericInfos = genericInfos;
    }

    void ClassDeclaration::add(const std::shared_ptr<SyntaxNode> &member) {
        _members.push_back(member);
    }

    void ClassDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, false);
        syntaxWalker(_inheritInfos, false);
        if(_members.empty())
            syntaxWalker(_genericInfos, true);
        else {
            syntaxWalker(_genericInfos, false);
            for (int i = 0; i < _members.size(); ++i) {
                syntaxWalker(_members[i], i == _members.size() - 1);
            }
        }
    }

    void ClassDeclaration::writeCurrentInfo(std::ostream &s) {

        std::ostringstream classData;
        classData << "'class "
                  << _name->toString()
                  << "<";
        _genericInfos->forEachChild([&classData](const std::weak_ptr<SyntaxNode>& n, bool isLast){
            classData << std::static_pointer_cast<GenericType>(n.lock())->toString()
                      << (isLast?">:":",");
        });
        _inheritInfos->forEachChild([&classData](const std::weak_ptr<SyntaxNode>& n, bool isLast){
            classData << std::static_pointer_cast<Type>(n.lock())->toString()
                      << (isLast?"'":",");
        });
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
}