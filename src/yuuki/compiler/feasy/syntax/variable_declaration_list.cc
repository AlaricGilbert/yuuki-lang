#include <yuuki/compiler/feasy/syntax/variable_declaration_list.h>
#include <rang/rang.h>
namespace yuuki::compiler::feasy::syntax{

    VariableDeclarationList::VariableDeclarationList(const std::shared_ptr<Type> &type) {
        _type = type;
    }

    void
    VariableDeclarationList::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_type, _varDecls.empty());
        for (std::size_t i = 0; i < _varDecls.size(); i++){
            syntaxWalker(_varDecls[i], i == _varDecls.size() - 1);
        }
    }

    void VariableDeclarationList::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "VariableDeclarationList "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "type: "
              << rang::fg::green    << "'" << _type->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "VariableDeclarationList "
              << "<" << this << "> "
              << "condition: "
              << "'" << _type->toString() << "'"
              << std::endl;
        }
    }

    void VariableDeclarationList::add(const std::shared_ptr<Expression> &varDecl) {
        _varDecls.push_back(varDecl);
    }

    SyntaxType VariableDeclarationList::getType() {
        return SyntaxType::VariableDeclarationList;
    }

    bool VariableDeclarationList::hasChild() {
        return true;
    }

    std::size_t VariableDeclarationList::start() {
        return _type->start();
    }

    std::size_t VariableDeclarationList::end() {
        if(_varDecls.empty())
            return _type->end();
        return _varDecls.back()->end();
    }
}