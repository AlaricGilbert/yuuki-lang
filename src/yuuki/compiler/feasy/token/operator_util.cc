#include "yuuki/compiler/feasy/token/operator_util.h"
namespace yuuki::compiler::feasy::token{

    bool OperatorUtil::isUnary(TokenType type) {
        switch (type){
            case TokenType::op_plusplus:
            case TokenType::op_plus:
            case TokenType::op_minusminus:
            case TokenType::op_minus:
            case TokenType::op_tilde:
            case TokenType::op_exclaim:
                return true;
            default:
                return false;
        }
    }

    bool OperatorUtil::isPrimary(TokenType type) {
        switch (type){
            case TokenType::l_paren:
            case TokenType::l_square:
            case TokenType::op_plusplus:
            case TokenType::op_minusminus:
                return true;
            default:
                return false;
        }
    }

    std::size_t OperatorUtil::getBinaryOperatorPrecedence(TokenType type) {
        switch (type){
            // Multiple
            case TokenType::op_multi:
            case TokenType::op_slash:
            case TokenType::op_modulus:
                return multiplicative;
            // Add
            case TokenType::op_plus:
            case TokenType::op_minus:
                return addition;
            // Shift
            case TokenType::op_greatergreater:
            case TokenType::op_lessless:
                return shift;
            // Relational
            case TokenType::op_greater:
            case TokenType::op_less:
            case TokenType::op_greatereq:
            case TokenType::op_lesseq:
                return relational;
            // Equality
            case TokenType::op_equaleq:
            case TokenType::op_exclaimeq:
                return equality;
            // Logical AND
            case TokenType::op_amp:
                return logical_and;
            // Logical XOR
            case TokenType::op_caret:
                return logical_xor;
            // Logical OR
            case TokenType::op_pipe:
                return logical_or;
            // Conditional AND
            case TokenType::op_ampamp:
                return conditional_and;
            // Conditional OR
            case TokenType::op_pipepipe:
                return conditional_or;
            // Assignment
            case TokenType::op_equal:
            case TokenType::op_multieq:
            case TokenType::op_slasheq:
            case TokenType::op_moduluseq:
            case TokenType::op_pluseq:
            case TokenType::op_minuseq:
            case TokenType::op_ampeq:
            case TokenType::op_careteq:
            case TokenType::op_pipeeq:
                return assignment;
        }
    }
}