#include "gtest/gtest.h"
#include "yuuki/compiler/feasy/lexer.h"
#include "yuuki/compiler/feasy/token/token_util.h"
using namespace yuuki::compiler::feasy;
using namespace yuuki::compiler::feasy::token;
using namespace yuuki::compiler::diagnostics;
TEST(Lexer,lexerSuccessLex) {
    std::string_view code = R"(
// sample code
import yuuki.core;

namespace yuuki.test.foo {
    // comment is allowed here
    public class full<T,T1> : map<list<T[][]>,T1> {
        /* inter-line comment is supported */
        public string toString(){
            if(1 <= 2){
                return "full2string\
which can switch line by this way";
            }
        }
    }
}
)";


    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<Diagnostic>(sm);
    auto context = sm->create(code);
    Lexer lexer = Lexer(context,d);
    lexer.lex();

    std::vector<TokenType> type_lists = {
            inline_comment,
            kw_import,
            identifier,
            op_period,
            identifier,
            semi,
            kw_namespace,
            identifier,
            op_period,
            identifier,
            op_period,
            identifier,
            l_brace,
            inline_comment,
            kw_public,
            kw_class,
            identifier,
            op_less,
            identifier,
            comma,
            identifier,
            op_greater,
            op_colon,
            identifier,
            op_less,
            identifier,
            op_less,
            identifier,
            l_square,
            r_square,
            l_square,
            r_square,
            op_greater,
            comma,
            identifier,
            op_greater,
            l_brace,
            interline_comment,
            kw_public,
            identifier,
            identifier,
            l_paren,
            r_paren,
            l_brace,
            kw_if,
            l_paren,
            numeric_const,
            op_lesseq,
            numeric_const,
            r_paren,
            l_brace,
            kw_return,
            string_const,
            semi,
            r_brace,
            r_brace,
            r_brace,
            r_brace,
            eof
    };
    std::vector<std::string> literal_vars = {
            "// sample code",
            "yuuki",
            "core",
            "yuuki",
            "test",
            "foo",
            "// comment is allowed here",
            "full",
            "T",
            "T1",
            "map",
            "list",
            "T",
            "T1",
            "/* inter-line comment is supported */",
            "string",
            "toString",
            "1",
            "2",
            "full2string\\\nwhich can switch line by this way"
    };
    int j = 0;
    for (int i = 0; i < 59; ++i) {
        auto type = context->tokens[i]->type;
        EXPECT_EQ(type,type_lists[i]);
        if (type == inline_comment || type == interline_comment || type == numeric_const || type == string_const ||
            type == identifier) {
            EXPECT_EQ(literal_vars[j++], (std::string)context->tokens[i]->rawCode);
        }
    }
}

TEST(Lexer,lexerFailLex) {
    std::string_view code = R"(
// sample code
import yuuki.core;

namespace yuuki.test.foo {
    // comment is allowed here
    public class full<T,T1> : map<list<T[][]>,T1> {
        /* inter-line comment is supported */
        public string toString(){
            if(1 <= 2){
                return " string not closed
            }
        }
        /* interline comment not closed
    }
}
)";


    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<Diagnostic>(sm);
    auto context = sm->create(code);
    Lexer lexer = Lexer(context,d);
    lexer.lex();

    std::vector<TokenType> type_lists = {
            inline_comment,
            kw_import,
            identifier,
            op_period,
            identifier,
            semi,
            kw_namespace,
            identifier,
            op_period,
            identifier,
            op_period,
            identifier,
            l_brace,
            inline_comment,
            kw_public,
            kw_class,
            identifier,
            op_less,
            identifier,
            comma,
            identifier,
            op_greater,
            op_colon,
            identifier,
            op_less,
            identifier,
            op_less,
            identifier,
            l_square,
            r_square,
            l_square,
            r_square,
            op_greater,
            comma,
            identifier,
            op_greater,
            l_brace,
            interline_comment,
            kw_public,
            identifier,
            identifier,
            l_paren,
            r_paren,
            l_brace,
            kw_if,
            l_paren,
            numeric_const,
            op_lesseq,
            numeric_const,
            r_paren,
            l_brace,
            kw_return,
            string_const,
            r_brace,
            r_brace,
            interline_comment,
            eof
    };
    std::vector<std::string> literal_vars = {
            "// sample code",
            "yuuki",
            "core",
            "yuuki",
            "test",
            "foo",
            "// comment is allowed here",
            "full",
            "T",
            "T1",
            "map",
            "list",
            "T",
            "T1",
            "/* inter-line comment is supported */",
            "string",
            "toString",
            "1",
            "2",
            "\" string not closed",
            "/* interline comment not closed\n    }\n}\n"
    };
    int j = 0;
    for (int i = 0; i < 57; ++i) {
        auto type = context->tokens[i]->type;
        EXPECT_EQ(type,type_lists[i]);
        if (i == 50)
            EXPECT_TRUE(true);
        if (type == inline_comment || type == interline_comment || type == numeric_const || type == string_const ||
            type == identifier) {
            EXPECT_EQ(literal_vars[j++], (std::string)context->tokens[i]->rawCode);
        }
    }
    d->printErrors();
}