#include <gtest/gtest.h>
#include <yuuki/compiler/feasy/syntax/unary_expression.h>
#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/syntax/identifier_expression.h>
#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <yuuki/compiler/feasy/token/token_type.h>
#include <yuuki/compiler/feasy/syntax/syntax_ostream_writer.h>
#include <memory>
#include <sstream>

TEST(SyntaxOStreamWriter,writeTest){

    using namespace yuuki::compiler::feasy::syntax;
    using namespace yuuki::compiler::feasy::token;
    auto block = std::make_shared<BlockStatement>();
    auto id1 = std::make_shared<IdentifierExpression>("identifier1");
    auto id2 = std::make_shared<IdentifierExpression>("identifier2");
    auto id3 = std::make_shared<IdentifierExpression>("identifier3");
    auto id4 = std::make_shared<IdentifierExpression>("identifier4");

    auto unary1 = std::make_shared<UnaryExpression>(TokenType::op_minus,id1);
    block->add(unary1);
    auto unary2 = std::make_shared<UnaryExpression>(TokenType::op_tilde,id2);
    auto binary = std::make_shared<BinaryExpression>(id3,TokenType::op_minus,unary2);
    block->add(binary);
    block->add(id4);
    SyntaxOStreamWriter::writeTo(std::cout,std::dynamic_pointer_cast<SyntaxTree>(block));
    std::ostringstream actual;
    std::ostringstream target;
    SyntaxOStreamWriter::writeTo(actual,std::dynamic_pointer_cast<SyntaxTree>(block));

    target << "BlockStatement <"<< block << "> \n"
           << "|-UnaryExpression <"<< unary1 << "> '-'\n"
           << "| `-IdentifierExpression <"<< id1 <<"> identifier1\n"
           << "|-BinaryExpression <"<<binary<< "> '-'\n"
           << "| |-IdentifierExpression <"<< id3 <<"> identifier3\n"
           << "| `-UnaryExpression <"<<unary2<<"> '~'\n"
           << "|   `-IdentifierExpression <"<<id2<<"> identifier2\n"
           << "`-IdentifierExpression <"<<id4<<"> identifier4\n";
    EXPECT_EQ(actual.str(),target.str());
}