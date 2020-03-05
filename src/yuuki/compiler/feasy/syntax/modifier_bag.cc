#include <yuuki/compiler/feasy/syntax/modifier_bag.h>
#include <rang/rang.h>
#include <yuuki/compiler/feasy/token/token_util.h>

namespace yuuki::compiler::feasy::syntax {

    ModifierMark::ModifierMark(token::TokenType type, size_t tokenID) {
        _type = type;
        _tokenID = tokenID;
    }

    void ModifierMark::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void ModifierMark::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "ModifierMark "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset
              << '\'' << token::TokenUtil::getSpell(_type) << '\''
              << std::endl;
        } else{
            s << "ModifierMark "
              << "<" << this << "> "
              << '\'' << token::TokenUtil::getSpell(_type) << '\'' << std::endl;
        }
    }

    SyntaxType ModifierMark::getType() {
        return SyntaxType::ModifierMark;
    }

    bool ModifierMark::hasChild() {
        return false;
    }

    std::string ModifierMark::toString() {
        return token::TokenUtil::getSpell(_type);
    }

    void ModifierBag::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for (std::size_t i = 0; i < _marks.size(); ++i) {
            syntaxWalker(_marks[i], i == _marks.size() - 1);
        }
    }

    void ModifierBag::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "ModifierBag "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "ModifierBag "
              << "<" << this << "> " << std::endl;
        }
    }

    SyntaxType ModifierBag::getType() {
        return SyntaxType::ModifierBag;
    }

    void ModifierBag::add(const std::shared_ptr<SyntaxNode> &child) {
        _marks.push_back(child);
    }

    bool ModifierBag::hasChild() {
        return !_marks.empty();
    }
}