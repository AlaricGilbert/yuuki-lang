#include <yuuki/compiler/feasy/syntax/syntax_node.h>
namespace yuuki::compiler::feasy::syntax{
    std::ostream & operator<<(std::ostream& s,SyntaxType type){
        switch (type){
            case SyntaxType::SyntaxUnit:
                s<<"SyntaxUnit";
                break;
            case SyntaxType::ImportDirective:
                s<<"ImportDirective";
                break;
            case SyntaxType::ModifierMark:
                s<<"ModifierMark";
                break;
            case SyntaxType::ModifierList:
                s<<"ModifierList";
                break;
            case SyntaxType::VariableDeclarationList:
                s<<"VariableDeclarationList";
                break;
            case SyntaxType::BlockStatement:
                s<<"BlockStatement";
                break;
            case SyntaxType::IfStatement:
                s<<"IfStatement";
                break;
            case SyntaxType::ForStatement:
                s<<"ForStatement";
                break;
            case SyntaxType::WhileStatement:
                s<<"WhileStatement";
                break;
            case SyntaxType::DoWhileStatement:
                s<<"DoWhileStatement";
                break;
            case SyntaxType::NopStatement:
                s<<"NopStatement";
                break;
            case SyntaxType::TryCatchStatement:
                s<<"TryCatchStatement";
                break;
            case SyntaxType::SwitchStatement:
                s<<"SwitchStatement";
                break;
            case SyntaxType::CaseStatement:
                s<<"CaseStatement";
                break;
            case SyntaxType::DefaultStatement:
                s<<"DefaultStatement";
                break;
            case SyntaxType::BreakStatement:
                s<<"BreakStatement";
                break;
            case SyntaxType::ContinueStatement:
                s<<"ContinueStatement";
                break;
            case SyntaxType::ReturnStatement:
                s<<"ReturnStatement";
                break;
            case SyntaxType::GotoStatement:
                s<<"GotoStatement";
                break;
            case SyntaxType::LabelStatement:
                s<<"LabelStatement";
                break;
            case SyntaxType::NamespaceDeclaration:
                s<<"NamespaceDeclaration";
                break;
            case SyntaxType::ClassDeclaration:
                s<<"ClassDeclaration";
                break;
            case SyntaxType::GenericDeclaration:
                s<<"GenericDeclaration";
                break;
            case SyntaxType::GenericArgumentList:
                s<<"GenericArgumentList";
                break;
            case SyntaxType::InheritDeclaration:
                s<<"InheritDeclaration";
                break;
            case SyntaxType::MethodDeclaration:
                s<<"MethodDeclaration";
                break;
            case SyntaxType::FieldDeclaration:
                s<<"FieldDeclaration";
                break;
            case SyntaxType::ParamDeclaration:
                s<<"ParamDeclaration";
                break;
            case SyntaxType::ParamList:
                s<<"ParamList";
                break;
            case SyntaxType::IdentifierName:
                s<<"IdentifierName";
                break;
            case SyntaxType::QualifiedName:
                s<<"QualifiedName";
                break;
            case SyntaxType::TrivialType:
                s<<"TrivialType";
                break;
            case SyntaxType::ArrayType:
                s<<"ArrayType";
                break;
            case SyntaxType::GenericType:
                s<<"GenericType";
                break;
            case SyntaxType::UnknownType:
                s<<"UnknownType";
                break;
            case SyntaxType::BinaryExpression:
                s<<"BinaryExpression";
                break;
            case SyntaxType::ExplicitCastExpression:
                s<<"ExplicitCastExpression";
                break;
            case SyntaxType::UnaryExpression:
                s<<"UnaryExpression";
                break;
            case SyntaxType::NameExpression:
                s<<"NameExpression";
                break;
            case SyntaxType::NullExpression:
                s<<"NullExpression";
                break;
            case SyntaxType::ThisExpression:
                s<<"ThisExpression";
                break;
            case SyntaxType::StringLiteralExpression:
                s<<"StringLiteralExpression";
                break;
            case SyntaxType::NumericLiteralExpression:
                s<<"NumericLiteralExpression";
                break;
            case SyntaxType::CallExpression:
                s<<"CallExpression";
                break;
            case SyntaxType::GenericCallExpression:
                s<<"GenericCallExpression";
                break;
            case SyntaxType::IndexExpression:
                s<<"IndexExpression";
                break;
            case SyntaxType::ParenthesesExpression:
                s<<"ParenthesesExpression";
                break;
            case SyntaxType::PostfixExpression:
                s<<"PostfixExpression";
                break;
            case SyntaxType::InlineComment:
                s<<"InlineComment";
                break;
            case SyntaxType::InterlineComment:
                s<<"InterlineComment";
                break;
            default:
                s<<"Unknown";
                break;
        }
        return s;
    }
}