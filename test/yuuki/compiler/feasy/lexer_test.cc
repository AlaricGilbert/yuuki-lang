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
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer lexer = Lexer(context,d);
    lexer.lex();

    std::vector<TokenType> type_lists = {
            TokenType::inline_comment,
            TokenType::kw_import,
            TokenType::identifier,
            TokenType::op_period,
            TokenType::identifier,
            TokenType::semi,
            TokenType::kw_namespace,
            TokenType::identifier,
            TokenType::op_period,
            TokenType::identifier,
            TokenType::op_period,
            TokenType::identifier,
            TokenType::l_brace,
            TokenType::inline_comment,
            TokenType::kw_public,
            TokenType::kw_class,
            TokenType::identifier,
            TokenType::op_less,
            TokenType::identifier,
            TokenType::comma,
            TokenType::identifier,
            TokenType::op_greater,
            TokenType::op_colon,
            TokenType::identifier,
            TokenType::op_less,
            TokenType::identifier,
            TokenType::op_less,
            TokenType::identifier,
            TokenType::l_square,
            TokenType::r_square,
            TokenType::l_square,
            TokenType::r_square,
            TokenType::op_greater,
            TokenType::comma,
            TokenType::identifier,
            TokenType::op_greater,
            TokenType::l_brace,
            TokenType::interline_comment,
            TokenType::kw_public,
            TokenType::identifier,
            TokenType::identifier,
            TokenType::l_paren,
            TokenType::r_paren,
            TokenType::l_brace,
            TokenType::kw_if,
            TokenType::l_paren,
            TokenType::numeric_const,
            TokenType::op_lesseq,
            TokenType::numeric_const,
            TokenType::r_paren,
            TokenType::l_brace,
            TokenType::kw_return,
            TokenType::string_const,
            TokenType::semi,
            TokenType::r_brace,
            TokenType::r_brace,
            TokenType::r_brace,
            TokenType::r_brace,
            TokenType::eof
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
        if (type == TokenType::inline_comment || type == TokenType::interline_comment ||
            type == TokenType::numeric_const || type == TokenType::string_const || type == TokenType::identifier) {
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
    auto d = std::make_shared<DiagnosticStream>(sm);
    auto context = sm->create(code);
    Lexer lexer = Lexer(context,d);
    lexer.lex();

    std::vector<TokenType> type_lists = {
            TokenType::inline_comment,
            TokenType::kw_import,
            TokenType::identifier,
            TokenType::op_period,
            TokenType::identifier,
            TokenType::semi,
            TokenType::kw_namespace,
            TokenType::identifier,
            TokenType::op_period,
            TokenType::identifier,
            TokenType::op_period,
            TokenType::identifier,
            TokenType::l_brace,
            TokenType::inline_comment,
            TokenType::kw_public,
            TokenType::kw_class,
            TokenType::identifier,
            TokenType::op_less,
            TokenType::identifier,
            TokenType::comma,
            TokenType::identifier,
            TokenType::op_greater,
            TokenType::op_colon,
            TokenType::identifier,
            TokenType::op_less,
            TokenType::identifier,
            TokenType::op_less,
            TokenType::identifier,
            TokenType::l_square,
            TokenType::r_square,
            TokenType::l_square,
            TokenType::r_square,
            TokenType::op_greater,
            TokenType::comma,
            TokenType::identifier,
            TokenType::op_greater,
            TokenType::l_brace,
            TokenType::interline_comment,
            TokenType::kw_public,
            TokenType::identifier,
            TokenType::identifier,
            TokenType::l_paren,
            TokenType::r_paren,
            TokenType::l_brace,
            TokenType::kw_if,
            TokenType::l_paren,
            TokenType::numeric_const,
            TokenType::op_lesseq,
            TokenType::numeric_const,
            TokenType::r_paren,
            TokenType::l_brace,
            TokenType::kw_return,
            TokenType::string_const,
            TokenType::r_brace,
            TokenType::r_brace,
            TokenType::interline_comment,
            TokenType::eof
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
        if (type == TokenType::inline_comment || type == TokenType::interline_comment ||
            type == TokenType::numeric_const || type == TokenType::string_const ||type == TokenType::identifier) {
            EXPECT_EQ(literal_vars[j++], (std::string)context->tokens[i]->rawCode);
        }
    }
    std::cout << *d;
}