#include "gtest/gtest.h"
#include "yuuki/compiler/feasy/lexer.h"
#include "yuuki/compiler/feasy/parser.h"
#include "yuuki/compiler/feasy/syntax/syntax_ostream_writer.h"
#include <memory>
#include <yuuki/compiler/feasy/token/token_type.h>

using namespace yuuki::compiler::feasy;
using namespace yuuki::compiler::feasy::token;
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

TEST(Parser,fillGenericParamList){
    auto codes = {"<T0,T1,T2>","<T0,T1, >","<T0,T1 T2>","< >","< , >", "<", "<T0"};
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    for(auto& code:codes) {
        auto context = sm->create(code);
        context->codePath = "INTERNAL_TEST/parseGenericInfoTest.yuk";
        Lexer l = Lexer(context, d);
        Parser p = Parser(context, d);
        l.lex();
        auto list = std::make_shared<GenericTypeList>();
        p.fillGenericTypeList(list);
    }
    std::cout << *d;
}

TEST(Parser,splitCurrentMultiCharOperator){
    auto code = ">>>>";
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer l = Lexer(context,d);
    Parser p = Parser(context,d);
    l.lex();
    p.splitCurrentMultiCharOperator();
    EXPECT_EQ(context->tokens[0]->type,TokenType::op_greater);
    EXPECT_EQ(context->tokens[1]->type,TokenType::op_greater);
    EXPECT_EQ(context->tokens[2]->type,TokenType::op_greatergreater);
}

TEST(Parser,parseType){
    auto code = "Complicated/*comment*/<yuuki.list< int32[/*comment*/]>,some/*comment*/.great.class1<T>>[][][]";
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer l = Lexer(context,d);
    Parser p = Parser(context,d);
    l.lex();
    auto t = p.parseType();
    std::cout << *t;
    std::cout << *d;
}

TEST(Parser,parseClassDecl){
    
    auto code = "public naive class list<T>:i_enumerable<T>,i_gc_ignored";
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer l = Lexer(context,d);
    Parser p = Parser(context,d);
    l.lex();
    auto t = p.parseClass();
    std::cout << *t;
    std::cout << *d;
}

TEST(Parser,parsePrecedenceExpr){
    auto code = "(vec2<int>)(a[x++][l].member * ~b.method(p[h],2.3,\"str\")).genMethod<int[],Type<T>>(1) +b1<b2>b3+ (c.def * m >> this)";
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer l = Lexer(context,d);
    Parser p = Parser(context,d);
    l.lex();
    auto t = p.parseExpression({});
    std::cout << *t;
    std::cout << *d;
}