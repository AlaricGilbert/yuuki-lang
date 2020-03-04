#include <yuuki/compiler/feasy/syntax/identifier_expression.h>

namespace yuuki::compiler::feasy::syntax{

    IdentifierExpression::IdentifierExpression(const std::string &id) {
        _identifier = id;
    }

    std::string IdentifierExpression::getIDString() {
        return _identifier;
    }

    void IdentifierExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>)> &syntaxWalker) {}
}