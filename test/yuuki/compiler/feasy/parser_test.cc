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
    
    auto code = R"(public naive class list<T>:i_enumerable<T>,i_gc_ignored{
                       public static int[][] matrix;
                       public static void main(string[] args){
                           console.log("hello,world!");
                       }
                   })";
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

TEST(Parser,parseField){
    auto codes = {
            "public vec2<Matrix<int>> a = 1,b =-2;",
            "public content<m> c = t,",
            "public content<r> m t"
    };
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    for(auto& code:codes) {
        auto context = sm->create(code);
        context->codePath = "INTERNAL_TEST/parseFieldTest.yuk";
        Lexer l = Lexer(context, d);
        Parser p = Parser(context, d);
        l.lex();
        std::cout << *p.parseFieldDeclaration();
    }
    std::cout << *d;
}

TEST(Parser,parseStatement){
    auto codes = {
            "if(a+b==c)while(true);else;",
            "break;",
            "return 1 + 2;",
            "return;",
            "while(p)break;",
            "continue;",
            "goto a;",
            "label_:",
            R"(
                {
                    double re = console.read().to_double(),
                           im = console.read().to_double();
                    complex r = complex._ctor(re,im);
                    switch(r.get_type())
                    {
                        case complex.type.real:
                        {
                            console.log("its' a real number!");
                            break;
                        }
                        case complex.type.imagine:
                        {
                            console.log("its' a pure imagine number!");
                            break;
                        }
                        case complex.type.complexed:
                        {
                            console.log("its' a complex number!");
                            break;
                        }
                    }
                }
)",
            R"(
for(int i = 1;i < int32.max();i++){
    console.log(math.log(i));
}

)"
    };
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    for(auto& code:codes) {
        auto context = sm->create(code);
        context->codePath = "INTERNAL_TEST/parseStatementTest.yuk";
        Lexer l = Lexer(context, d);
        Parser p = Parser(context, d);
        l.lex();
        std::cout << *p.parseStatement();
    }
    std::cout << *d;
}

TEST(Parser,parseMethodDecl){
    auto codes = {
            "public pair<K,V> getResult<K,V>(int a = 1,int b){return make_pair<K,V>(1,2);}"
    };
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    for(auto& code:codes) {
        auto context = sm->create(code);
        context->codePath = "INTERNAL_TEST/parseMethodDecl.yuk";
        Lexer l = Lexer(context, d);
        Parser p = Parser(context, d);
        l.lex();
        std::cout << *p.parseMethodDeclaration();
    }
    std::cout << *d;
}

TEST(Parser,parse){
    auto code =
R"(
import yuuki.std;

namespace foo.bar{
    class program{
        public static void main(string[] args){
            console.log("Hello, world!");
        }
    }
}
)";
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer l = Lexer(context,d);
    Parser p = Parser(context,d);
    l.lex();
    p.parse();
    std::cout << *(*sm)[0]->syntaxTree;
    std::cout << *d;
}