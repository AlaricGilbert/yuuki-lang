#include "gtest/gtest.h"
#include "yuuki/compiler/feasy/token/token_util.h"
#include "yuuki/compiler/feasy/token/token_type.h"

TEST(TokenUtilTest,getTypeTest) {
    using namespace yuuki::compiler::feasy::token;
    TokenType t0 = TokenUtil::getType("&&");
    TokenType t1 = TokenUtil::getType("public");
    TokenType t2 = TokenUtil::getType("{");
    TokenType t3 = TokenUtil::getType("continue");

    EXPECT_EQ(t0, op_ampamp);
    EXPECT_EQ(t1, kw_public);
    EXPECT_EQ(t2, l_brace);
    EXPECT_EQ(t3, kw_continue);
}

TEST(TokenUtilTest,getSpellTest){

    using namespace yuuki::compiler::feasy::token;
    auto* sp0 = TokenUtil::getSpell(op_minuseq);
    auto* sp1 = TokenUtil::getSpell(kw_override);
    auto* sp2 = TokenUtil::getSpell(r_paren);
    auto* sp3 = TokenUtil::getSpell(kw_break);

    EXPECT_STREQ(sp0,"-=");
    EXPECT_STREQ(sp1,"override");
    EXPECT_STREQ(sp2,")");
    EXPECT_STREQ(sp3,"break");

}