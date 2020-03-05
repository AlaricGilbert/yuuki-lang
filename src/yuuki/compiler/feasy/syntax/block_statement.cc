#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    void BlockStatement::add(const std::shared_ptr<SyntaxNode> &child) {
        _children.push_back(child);
    }

    void BlockStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "BlockStatement "
              << rang::fg::yellow   << "<" << this << "> " << std::endl;
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
}