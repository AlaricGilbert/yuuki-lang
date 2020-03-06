#ifndef YUUKI_MODIFIER_LIST_H
#define YUUKI_MODIFIER_LIST_H
#include <yuuki/compiler/feasy/syntax/import_directive.h>
#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::syntax {
    class ModifierMark: public SyntaxNode{
    public:
        ModifierMark(token::TokenType type, size_t tokenID);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::string toString();

    private:
        token::TokenType _type;
        std::size_t  _tokenID;
    };
    class ModifierList: public SyntaxNode, public ISyntaxList<ModifierMark>{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        void add(const std::shared_ptr<ModifierMark>& child) override ;
    private:
        std::vector<std::shared_ptr<ModifierMark>> _marks;
    };
}
#endif //YUUKI_MODIFIER_LIST_H
