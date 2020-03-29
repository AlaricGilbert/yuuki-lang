#include <yuuki/compiler/feasy/syntax/class_declaration.h>
#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <rang/rang.h>
#include <sstream>
#include <memory>

namespace yuuki::compiler::feasy::syntax{

    ClassDeclaration::ClassDeclaration(const std::shared_ptr<ModifierList> &mod,
                                       const std::shared_ptr<Name> &name,
                                       const std::shared_ptr<GenericTypeList> &genericInfos,
                                       const std::shared_ptr<InheritTypeList> &inheritInfos) {
        _mod = mod;
        _name = name;
        _inheritInfos = inheritInfos;
        _genericInfos = genericInfos;
        _classTokenIndex = invalidTokenIndex;
    }

    void ClassDeclaration::add(const std::shared_ptr<MethodDeclaration> &method) {
        _members.push_back(method);
    }

    void ClassDeclaration::add(const std::shared_ptr<FieldDeclaration> &field) {
        _members.push_back(field);
    }

    void ClassDeclaration::add(const std::shared_ptr<ClassDeclaration> &child) {
        _members.push_back(child);
    }

    void ClassDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, false);
        if(_mod->hasChild())
            syntaxWalker(_mod, false);
        if(_inheritInfos->hasChild())
            syntaxWalker(_inheritInfos, false);
        if(_genericInfos->hasChild())
            syntaxWalker(_genericInfos, _members.empty());
        for (std::size_t i = 0; i < _members.size(); ++i) {
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

    std::size_t ClassDeclaration::start() {
        if(_mod->hasChild())
            return _mod->start();
        if(_classTokenIndex != invalidTokenIndex)
            return _classTokenIndex;
        return _name->start();
    }

    std::size_t ClassDeclaration::end() {
        if(_rBraceIndex == invalidTokenIndex){
            if(_members.empty()){
                return _lBraceIndex;
            }
            return _members.back()->end();
        }
        return _rBraceIndex;
    }

    void ClassDeclaration::setClassTokenIndex(std::size_t classTokenIndex) {
        _classTokenIndex = classTokenIndex;
    }

    void ClassDeclaration::setLBraceIndex(std::size_t lBraceIndex) {
        _lBraceIndex = lBraceIndex;
    }

    void ClassDeclaration::setRBraceIndex(std::size_t rBraceIndex) {
        _rBraceIndex = rBraceIndex;
    }

    void ClassDeclaration::forEachMember(const std::function<void (std::weak_ptr<SyntaxNode>)> &syntaxWalker) {
        for (const auto & _member : _members) {
            syntaxWalker(_member);
        }
    }

    std::size_t ClassDeclaration::getClassTokenIndex() const {
        return _classTokenIndex;
    }

    std::size_t ClassDeclaration::getLBraceIndex() const {
        return _lBraceIndex;
    }

    std::size_t ClassDeclaration::getRBraceIndex() const {
        return _rBraceIndex;
    }

    std::shared_ptr<ModifierList>ClassDeclaration::getMod() {
        return _mod;
    }

    std::shared_ptr<Name> ClassDeclaration::getName() {
        return _name;
    }

    std::shared_ptr<InheritTypeList> ClassDeclaration::getInheritInfos() {
        return _inheritInfos;
    }

    std::shared_ptr<GenericTypeList> ClassDeclaration::getGenericInfos() {
        return _genericInfos;
    }

}