#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    void BlockStatement::add(const std::shared_ptr<Statement> &child) {
        _children.push_back(child);
    }

    void BlockStatement::add(const std::shared_ptr<Expression> &child) {
        _children.push_back(child);
    }
    void BlockStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "BlockStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "BlockStatement "
              << "<" << this << "> " << std::endl;
        }
    }

    void BlockStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _children.size();id++){
            syntaxWalker(_children[id], id == _children.size() - 1);
        }
    }

    SyntaxType BlockStatement::getType() {
        return SyntaxType::BlockStatement;
    }

    bool BlockStatement::hasChild() {
        return !_children.empty();
    }

}