#include "gtest/gtest.h"
#include "yuuki/compiler/feasy/lexer.h"
#include "yuuki/compiler/feasy/parser.h"
#include "yuuki/compiler/feasy/syntax/syntax_ostream_writer.h"
#include <memory>
using namespace yuuki::compiler::feasy;
using namespace yuuki::compiler::feasy::syntax;
using namespace yuuki::compiler::diagnostics;
TEST(Parser,parseName){
    std::string_view code1 = R"(yuuki//blabla
./*23333*/core.test)";
    std::string_view code2 = R"(yuuki.core./*23333*/)";
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context1 = sm->create(code1);
    auto context2 = sm->create(code2);
    Lexer lexer1 = Lexer(context1,d);
    Lexer lexer2= Lexer(context2,d);
    lexer1.lex();
    lexer2.lex();
    Parser p1 = Parser(context1,d);
    Parser p2 = Parser(context2,d);
    auto tree1 = p1.parseName();
    auto tree2 = p2.parseName();
    std::cout << *tree1;
    std::cout << *tree2;
}

TEST(Parser,parseGenericInfo){
    auto codes = {"<T0,T1,T2>","<T0,T1, >","<T0,T1 T2>","< >","< , >"};
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    for(auto& code:codes) {
        auto context = sm->create(code);
        context->codePath = "INTERNAL_TEST/parseGenericInfoTest.yuk";
        Lexer l = Lexer(context, d);
        Parser p = Parser(context, d);
        l.lex();
        auto gen = p.parseGenericInfo();
        std::cout <<*gen;
    }
    std::cout << *d;
}