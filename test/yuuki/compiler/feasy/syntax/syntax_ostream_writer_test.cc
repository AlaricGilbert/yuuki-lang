#include <gtest/gtest.h>
#include <yuuki/compiler/feasy/syntax/unary_expression.h>
#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/type.h>
#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <yuuki/compiler/feasy/token/token_type.h>
#include <yuuki/compiler/feasy/syntax/syntax_ostream_writer.h>
#include <memory>
#include <sstream>
#include <yuuki/compiler/feasy/syntax/class_declaration.h>

TEST(SyntaxOStreamWriter,writeTest){

    using namespace yuuki::compiler::feasy::syntax;
    using namespace yuuki::compiler::feasy::token;
    auto block = std::make_shared<BlockStatement>();
    auto id1 = std::make_shared<IdentifierName>("identifier1");
    auto id2 = std::make_shared<IdentifierName>("identifier2");
    auto id3 = std::make_shared<IdentifierName>("identifier3");
    auto id4 = std::make_shared<IdentifierName>("identifier4");
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
           << "| `-IdentifierName <"<< id1 <<"> 'identifier1'\n"
           << "|-BinaryExpression <"<<binary<< "> '-'\n"
           << "| |-IdentifierName <"<< id3 <<"> 'identifier3'\n"
           << "| `-UnaryExpression <"<<unary2<<"> '~'\n"
           << "|   `-IdentifierName <"<<id2<<"> 'identifier2'\n"
           << "`-IdentifierName <"<<id4<<"> 'identifier4'\n";
    EXPECT_EQ(actual.str(),target.str());
    auto type1 = std::make_shared<TrivialType>(id1);
    auto type2 = std::make_shared<ArrayType>(type1);
    auto type3 = std::make_shared<GenericType>(id2);
    auto type4 = std::make_shared<GenericType>(id3);
    type3->addGenericType(type1);
    type3->addGenericType(type2);
    type4->addGenericType(type1);
    type4->addGenericType(type2);
    auto gene = std::make_shared<GenericDeclaration>();
    gene->addGenericType(id1);
    gene->addGenericType(id2);
    gene->addGenericType(id3);
    auto inherits = std::make_shared<InheritDeclaration>();
    inherits->addInheritType(type3);
    inherits->addInheritType(type4);
    auto mod = std::make_shared<ModifierBag>();
    auto class1 = std::make_shared<ClassDeclaration>(mod,id4,inherits,gene);
    SyntaxOStreamWriter::writeTo(std::cout,class1);

}