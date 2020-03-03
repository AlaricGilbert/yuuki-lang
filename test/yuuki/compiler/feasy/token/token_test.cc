#include "gtest/gtest.h"
#include "yuuki/compiler/feasy/token/token.h"
TEST(Token,isTest){
    using namespace yuuki::compiler::feasy::token;
    Token tok;
    tok.type = kw_import;
    EXPECT_FALSE(tok.is(modifiers));
    EXPECT_TRUE(tok.is(keywords));
    EXPECT_FALSE(tok.is(punctuator));
    EXPECT_FALSE(tok.is(operators));
    tok.type = kw_private;
    EXPECT_TRUE(tok.is(modifiers));
    EXPECT_TRUE(tok.is(keywords));
    EXPECT_FALSE(tok.is(punctuator));
    EXPECT_FALSE(tok.is(operators));
    tok.type = r_paren;
    EXPECT_FALSE(tok.is(modifiers));
    EXPECT_FALSE(tok.is(keywords));
    EXPECT_TRUE(tok.is(punctuator));
    EXPECT_FALSE(tok.is(operators));
    tok.type = op_ampamp;
    EXPECT_FALSE(tok.is(modifiers));
    EXPECT_FALSE(tok.is(keywords));
    EXPECT_FALSE(tok.is(punctuator));
    EXPECT_TRUE(tok.is(operators));
}