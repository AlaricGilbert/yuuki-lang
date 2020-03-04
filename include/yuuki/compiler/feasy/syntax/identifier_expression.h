#ifndef YUUKI_IDENTIFIER_EXPRESSION_H
#define YUUKI_IDENTIFIER_EXPRESSION_H

#include <yuuki/compiler/feasy/syntax/expression.h>
#include <string>
namespace yuuki::compiler::feasy::syntax{
    class IdentifierExpression: public Expression{
    public:
        explicit IdentifierExpression(const std::string& id);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>)> &syntaxWalker) override;
        std::string getIDString();
    private:
        std::string _identifier;
    };
}
#endif //YUUKI_IDENTIFIER_EXPRESSION_H
