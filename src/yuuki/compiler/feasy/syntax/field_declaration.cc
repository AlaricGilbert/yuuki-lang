#include <yuuki/compiler/feasy/syntax/field_declaration.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{
    void FieldDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_modifiers, false);
        syntaxWalker(_varDecl, true);
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

    FieldDeclaration::FieldDeclaration(const std::shared_ptr<ModifierList>& modifiers,
            const std::shared_ptr<VariableDeclarationStatement>& varDecl) {
        _varDecl = varDecl;
        _modifiers = modifiers;
    }

    std::size_t FieldDeclaration::start() {
        if(_modifiers->hasChild())
            return _modifiers->start();
        return _varDecl->start();
    }

    std::size_t FieldDeclaration::end() {
        return  _varDecl->end();
    }
}