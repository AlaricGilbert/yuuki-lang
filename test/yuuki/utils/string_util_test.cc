#include "gtest/gtest.h"
#include "yuuki/utils/string_util.h"

TEST(StringUtil,unescape){
    std::string u8_tst = "UTF-8\\u6D4B\\u8BD5";
    std::string usual_tst = "\\a\\b\\t\\n test\\\n no newline";
    std::string o;
    int pos = 0;
    using namespace yuuki::utils;
    EXPECT_TRUE(StringUtil::unescape(u8_tst,o,pos));
    EXPECT_EQ(o,"UTF-8测试");
    EXPECT_EQ(pos,0);
    EXPECT_TRUE(StringUtil::unescape(usual_tst,o,pos));
    EXPECT_EQ(o,"\a\b\t\n test no newline");
    EXPECT_EQ(pos,0);
}