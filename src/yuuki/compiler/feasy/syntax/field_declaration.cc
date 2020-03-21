#include <yuuki/compiler/feasy/syntax/field_declaration.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{
    void FieldDeclaration::setSemiTokenIndex(std::size_t semiTokenIndex){
        _semiTokenIndex = semiTokenIndex;
    }


    void FieldDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_modifiers, false);
        syntaxWalker(_varDeclList, true);
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
        return true;
    }

    SyntaxType FieldDeclaration::getType() {
        return SyntaxType::FieldDeclaration;
    }

    FieldDeclaration::FieldDeclaration(const std::shared_ptr<ModifierList>& modifiers,const std::shared_ptr<Type>& type) {
        _varDeclList = std::make_shared<VariableDeclarationList>(type);
        _modifiers = modifiers;
        _semiTokenIndex = invalidTokenIndex;
    }

    std::size_t FieldDeclaration::start() {
        return _modifiers->start();
    }

    std::size_t FieldDeclaration::end() {
        if(_semiTokenIndex!=invalidTokenIndex)
            return _semiTokenIndex;
        return  _varDeclList->end();
    }

    std::shared_ptr<VariableDeclarationList> FieldDeclaration::getVarDeclList() {
        return _varDeclList;
    }
}