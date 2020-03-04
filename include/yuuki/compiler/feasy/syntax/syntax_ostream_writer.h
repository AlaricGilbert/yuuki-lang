//
// Created by Alaric on 2020/3/4.
//

#ifndef YUUKI_SYNTAX_STREAM_WRITER_H
#define YUUKI_SYNTAX_STREAM_WRITER_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <ostream>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{

    class SyntaxOStreamWriter{
    public:
        static void writeTo(std::ostream& s,const std::weak_ptr<SyntaxNode>& node) {
            auto nLock = node.lock();
            nLock->writeCurrentInfo(s);
            nLock->forEachChild([&](const std::weak_ptr<SyntaxNode>& n, bool isChildLast)->void{
                writeToChild(s,n,"",isChildLast);
            });
        }

    private:
        static void writeToChild(std::ostream& s,const std::weak_ptr<SyntaxNode>& node,
                            std::string intend = "",bool isLast = true) {
            bool toConsole = s.rdbuf()==std::cout.rdbuf();

            if (toConsole) {
                s << rang::fg::blue << intend << (isLast ? "`-" : "|-") << rang::fg::reset;
            } else{
                s << intend << (isLast ? "`-" : "|-");
            }
            intend += isLast?"  ":"| ";
            auto nLock = node.lock();
            nLock->writeCurrentInfo(s);
            nLock->forEachChild([&](const std::weak_ptr<SyntaxNode>& n, bool isChildLast)->void{
                writeToChild(s,n,intend,isChildLast);
            });
        }
    };
}
#endif //YUUKI_SYNTAX_STREAM_WRITER_H
