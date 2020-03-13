#include <yuuki/compiler/feasy/syntax/class_declaration.h>
#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <rang/rang.h>
#include <sstream>
#include <memory>

namespace yuuki::compiler::feasy::syntax{

    ClassDeclaration::ClassDeclaration(const std::shared_ptr<ModifierList> &mod,
                                       const std::shared_ptr<Name> &name,
                                       const std::shared_ptr<GenericDeclaration> &genericInfos,
                                       const std::shared_ptr<InheritDeclaration> &inheritInfos) {
        _mod = mod;
        _name = name;
        _inheritInfos = inheritInfos;
        _genericInfos = genericInfos;
        _classTokenIndex = invalidTokenIndex;
        _semiTokenIndex = invalidTokenIndex;
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

    void ClassDeclaration::setClassTokenIndex(std::size_t classTokenIndex) {
        _classTokenIndex = classTokenIndex;
    }

    std::size_t ClassDeclaration::start() {
        if(_mod->hasChild())
            return _mod->start();
        if(_classTokenIndex != invalidTokenIndex)
            return _classTokenIndex;
        return _name->start();
    }

    std::size_t ClassDeclaration::end() {
        if(_members.back()->end()!=invalidTokenIndex){
            if(_semiTokenIndex != invalidTokenIndex)
                return _semiTokenIndex;
            if(_inheritInfos->end() != invalidTokenIndex)
                return _inheritInfos->end();
            if(_genericInfos->end() != invalidTokenIndex)
                return _genericInfos->end();
            return _name->end();
        }

        return _members.back()->end();
    }

    void ClassDeclaration::setSemiTokenIndex(std::size_t semiTokenIndex) {
        _semiTokenIndex = semiTokenIndex;
    }

    void FieldDeclaration::add(const std::shared_ptr<Expression> &declExpr) {
        _fieldDeclExprs.push_back(declExpr);
    }

    void FieldDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(std::size_t i = 0; i < _fieldDeclExprs.size();i++){
            syntaxWalker(_fieldDeclExprs[i],i==_fieldDeclExprs.size() - 1);
        }
    }

    void FieldDeclaration::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "FieldDeclaration "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "FieldDeclaration "
              << "<" << this << "> " << std::endl;
        }
    }

    bool FieldDeclaration::hasChild() {
        return !_fieldDeclExprs.empty();
    }

    SyntaxType FieldDeclaration::getType() {
        return SyntaxType::FieldDeclaration;
    }
}