#include <yuuki/compiler/feasy/syntax/modifier_list.h>
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

    std::size_t ModifierMark::start() {
        return _tokenID;
    }

    std::size_t ModifierMark::end() {
        return _tokenID;
    }

    void ModifierList::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for (std::size_t i = 0; i < _marks.size(); ++i) {
            syntaxWalker(_marks[i], i == _marks.size() - 1);
        }
    }

    void ModifierList::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "ModifierList "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
        } else{
            s << "ModifierList "
              << "<" << this << "> " << std::endl;
        }
    }

    SyntaxType ModifierList::getType() {
        return SyntaxType::ModifierList;
    }

    void ModifierList::add(const std::shared_ptr<ModifierMark> &child) {
        _marks.push_back(child);
    }

    bool ModifierList::hasChild() {
        return !_marks.empty();
    }

    std::size_t ModifierList::start() {
        if(_marks.empty())
            return invalidTokenIndex;
        return _marks.front()->start();
    }

    std::size_t ModifierList::end() {
        if(_marks.empty())
            return invalidTokenIndex;
        return _marks.back()->end();
    }
}