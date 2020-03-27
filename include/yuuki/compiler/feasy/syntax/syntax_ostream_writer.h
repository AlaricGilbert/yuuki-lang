#ifndef YUUKI_SYNTAX_STREAM_WRITER_H
#define YUUKI_SYNTAX_STREAM_WRITER_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>


namespace yuuki::compiler::feasy::syntax{

    class SyntaxOStreamWriter {
    public:
        static void writeTo(std::ostream &s, const std::weak_ptr<SyntaxNode> &node);
        static void writeTo(std::ostream &s, SyntaxNode &node);

    private:
        static void writeToChild(std::ostream &s, const std::weak_ptr<SyntaxNode> &node,
                                 std::string intend = "", bool isLast = true);
    };
    std::ostream &operator<<(std::ostream &ostream,SyntaxNode& node);
}
#endif //YUUKI_SYNTAX_STREAM_WRITER_H
