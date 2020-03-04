#include <yuuki/compiler/feasy/syntax/identifier_expression.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    IdentifierExpression::IdentifierExpression(const std::string &id) {
        _identifier = id;
    }

    std::string IdentifierExpression::getIDString() {
        return _identifier;
    }

    void IdentifierExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {}

    void IdentifierExpression::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray    << "IdentifierExpression "
              << rang::fg::yellow  << "<" << this << "> "
              << rang::style::bold << rang::fg::reset
              << _identifier       << rang::style::reset << std::endl;
        } else{
            s << "IdentifierExpression "
              << "<" << this << "> "
              << _identifier << std::endl;
        }
    }

    void IdentifierExpression::analyseType() {

    }
}