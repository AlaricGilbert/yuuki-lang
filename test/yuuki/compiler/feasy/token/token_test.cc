#include "gtest/gtest.h"
#include "yuuki/compiler/feasy/token/token.h"
TEST(Token,isTest){
    using namespace yuuki::compiler::feasy::token;
    Token tok;
    tok.type = TokenType::kw_import;
    EXPECT_FALSE(tok.is(TokenType::modifiers));
    EXPECT_TRUE(tok.is(TokenType::keywords));
    EXPECT_FALSE(tok.is(TokenType::punctuator));
    EXPECT_FALSE(tok.is(TokenType::operators));
    tok.type = TokenType::kw_private;
    EXPECT_TRUE(tok.is(TokenType::modifiers));
    EXPECT_TRUE(tok.is(TokenType::keywords));
    EXPECT_FALSE(tok.is(TokenType::punctuator));
    EXPECT_FALSE(tok.is(TokenType::operators));
    tok.type = TokenType::r_paren;
    EXPECT_FALSE(tok.is(TokenType::modifiers));
    EXPECT_FALSE(tok.is(TokenType::keywords));
    EXPECT_TRUE(tok.is(TokenType::punctuator));
    EXPECT_FALSE(tok.is(TokenType::operators));
    tok.type = TokenType::op_ampamp;
    EXPECT_FALSE(tok.is(TokenType::modifiers));
    EXPECT_FALSE(tok.is(TokenType::keywords));
    EXPECT_FALSE(tok.is(TokenType::punctuator));
    EXPECT_TRUE(tok.is(TokenType::operators));
}