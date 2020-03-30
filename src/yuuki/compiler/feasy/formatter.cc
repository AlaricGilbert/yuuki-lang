#include <yuuki/compiler/feasy/formatter.h>
#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/token/token_util.h>
#include <yuuki/compiler/feasy/syntax/unary_expression.h>

namespace yuuki::compiler::feasy {

    std::string Formatter::format() {
        _codeBuffer.str("");
        _tokenIndex = 0;
        writeNode(_context->syntaxTree, 0);
        return _codeBuffer.str();
    }

    void Formatter::writeNode(const std::shared_ptr<syntax::SyntaxNode> &node, std::size_t indent) {
        if(node == nullptr)
            return;
        if (node->start() == syntax::SyntaxNode::invalidTokenIndex)
            return;
        switch (node->getType()) {

            case syntax::SyntaxType::NameExpression:
            case syntax::SyntaxType::FieldDeclaration:
            case syntax::SyntaxType::GenericType:
            case syntax::SyntaxType::TrivialType:
            case syntax::SyntaxType::ModifierList:
            case syntax::SyntaxType::ParamList:
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                });
                break;
            case syntax::SyntaxType::SyntaxUnit:
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                    _codeBuffer << std::endl;
                });
                break;
            case syntax::SyntaxType::ImportDirective:
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << " ";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ";";
                _tokenIndex++;
                break;
            case syntax::SyntaxType::ModifierMark:
                while (_tokenIndex < node->start()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << " ";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                });
                break;
            case syntax::SyntaxType::IfStatement:{
                auto ifStmt = std::static_pointer_cast<syntax::IfStatement>(node);
                while (_tokenIndex < ifStmt->getIfTokenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << " ";
                _tokenIndex++;
                while (_tokenIndex <ifStmt->getLParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                writeNode(ifStmt->getCondition(), indent);
                while (_tokenIndex <ifStmt->getRParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                writeNode(ifStmt->getIf(),indent);
                if(ifStmt->getElseTokenIndex() != syntax::SyntaxNode::invalidTokenIndex){
                    _codeBuffer << " ";
                    while (_tokenIndex <ifStmt->getElseTokenIndex()){
                        writeCurrentTokenAsComment(indent);
                    }
                    _codeBuffer << "else ";
                    _tokenIndex++;
                    writeNode(ifStmt->getElse(), indent);
                }
                return;
            }
            case syntax::SyntaxType::WhileStatement:{
                auto whileStmt = std::static_pointer_cast<syntax::WhileStatement>(node);
                while (_tokenIndex <whileStmt->getWhileTokenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << " ";
                _tokenIndex++;
                while (_tokenIndex <whileStmt->getLParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                writeNode(whileStmt->getCondition(), indent);
                while (_tokenIndex <whileStmt->getRParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                writeNode(whileStmt->getBody(),indent);

                return;
            }
            case syntax::SyntaxType::SwitchStatement:{
                auto switchStmt = std::static_pointer_cast<syntax::SwitchStatement>(node);
                while (_tokenIndex <switchStmt->getSwitchTokenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << " ";
                _tokenIndex++;
                while (_tokenIndex <switchStmt->getLParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                writeNode(switchStmt->getValue(), indent);
                while (_tokenIndex <switchStmt->getRParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                writeNode(switchStmt->getCases(), indent);

                return;
            }
            case syntax::SyntaxType::CaseStatement:{
                auto caseStmt = std::static_pointer_cast<syntax::CaseStatement>(node);
                while (_tokenIndex <caseStmt->getCaseTokenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "case ";
                _tokenIndex++;
                writeNode(caseStmt->getValue(), indent);
                while (_tokenIndex <caseStmt->getColonTokenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ": ";
                _tokenIndex++;
                writeNode(caseStmt->getCaseBlock(), indent);
                return;
            }
            case syntax::SyntaxType::ForStatement:{
                auto forStmt = std::static_pointer_cast<syntax::ForStatement>(node);
                while (_tokenIndex < forStmt->getForTokenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "for ";
                _tokenIndex++;
                while (_tokenIndex < forStmt->getLParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                writeNode(forStmt->getInit(), indent);
                writeNode(forStmt->getCondition(), indent);
                while (_tokenIndex < forStmt->getConditionEndSemiIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ";";
                _tokenIndex++;
                writeNode(forStmt->getPost(), indent);
                while (_tokenIndex < forStmt->getRParenIndex()){
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                writeNode(forStmt->getBody(), indent);
                return;
            }
            case syntax::SyntaxType::DoWhileStatement:
            case syntax::SyntaxType::TryCatchStatement:
                while (_tokenIndex < node->start()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << " ";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                });

                break;
            case syntax::SyntaxType::ExpressionStatement:
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ";";
                _tokenIndex++;
                break;

                // keyword xxx;\r\n
            case syntax::SyntaxType::ReturnStatement:
            case syntax::SyntaxType::GotoStatement:
            case syntax::SyntaxType::BreakStatement:
            case syntax::SyntaxType::ContinueStatement:
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode <<" ";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent + 4);
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ";";
                _tokenIndex++;
                break;
            case syntax::SyntaxType::BlockStatement:
                _codeBuffer << "{" << std::endl;
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {

                    writeIndent(indent + 4);
                    writeNode(name.lock(), indent + 4);
                    _codeBuffer << std::endl;
                });
                writeIndent(indent);
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "}";
                _tokenIndex++;
                break;
            case syntax::SyntaxType::NopStatement:
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ";";
                _tokenIndex++;
                break;
            case syntax::SyntaxType::DefaultStatement:
                while (_tokenIndex < node->start()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "default:";
                _tokenIndex += 2;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                });
                break;
            case syntax::SyntaxType::LabelStatement:
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent + 4);
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ":";
                return;
            case syntax::SyntaxType::VariableDeclarationStatement: {
                auto varDecl = std::static_pointer_cast<syntax::VariableDeclarationStatement>(node);
                writeNode(varDecl->getVariableType(), indent);
                _codeBuffer << " ";
                varDecl->forEachVarDecl([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                    if (!end) {
                        _codeBuffer << ", ";
                        _tokenIndex++;
                    }
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ";";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::GenericTypeList:
            case syntax::SyntaxType::GenericArgumentList:

                while (_tokenIndex < node->start()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "<";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                    if (!end) {
                        _codeBuffer << ", ";
                        _tokenIndex++;
                    }
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ">";
                _tokenIndex++;
                break;
            case syntax::SyntaxType::InheritDeclaration:
                _codeBuffer << ":";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent);
                    if (!end) {
                        _codeBuffer << ", ";
                        _tokenIndex++;
                    }
                });
                break;
            case syntax::SyntaxType::ParenthesesExpression:
                _codeBuffer << "(";
                _tokenIndex++;
                node->forEachChild([&](auto name, auto end) -> auto {
                    writeNode(name.lock(), indent + 4);
                });
                while (_tokenIndex < node->end()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                break;
            case syntax::SyntaxType::NamespaceDeclaration: {
                auto nsNode = std::static_pointer_cast<syntax::NamespaceDeclaration>(node);
                auto nsName = nsNode->getNamespaceName();

                while (_tokenIndex < nsNode->start()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "namespace ";
                _tokenIndex++;
                writeNode(nsName, indent);

                while (_tokenIndex < nsNode->getLBraceTokenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "{" << std::endl;
                _tokenIndex++;

                nsNode->forEachClass([&](auto child, auto end) -> auto {
                    writeIndent(indent + 4);
                    writeNode(child.lock(), indent + 4);
                    _codeBuffer << std::endl;
                });

                while (_tokenIndex < nsNode->getRBraceTokenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }

                writeIndent(indent);
                _codeBuffer << "}";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::ClassDeclaration: {
                auto classDecl = std::static_pointer_cast<syntax::ClassDeclaration>(node);
                writeNode(classDecl->getMod(), indent);

                while (_tokenIndex < classDecl->getClassTokenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "class ";
                _tokenIndex++;
                writeNode(classDecl->getName(), indent);
                writeNode(classDecl->getGenericInfos(), indent);
                writeNode(classDecl->getInheritInfos(), indent);


                while (_tokenIndex < classDecl->getLBraceIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "{" << std::endl;
                _tokenIndex++;
                classDecl->forEachMember([&](auto member) -> auto {
                    writeIndent(indent + 4); //
                    writeNode(member.lock(), indent + 4);
                    _codeBuffer << std::endl;
                });

                while (_tokenIndex < classDecl->getRBraceIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                writeIndent(indent);
                _codeBuffer << "}";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::MethodDeclaration: {
                auto method = std::static_pointer_cast<syntax::MethodDeclaration>(node);
                writeNode(method->getMod(), indent);
                writeNode(method->getReturnType(), indent);
                _codeBuffer << " ";
                writeNode(method->getName(), indent);
                writeNode(method->getGenericInfos(), indent);

                while (_tokenIndex < method->getLParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                writeNode(method->getParams(), indent);


                while (_tokenIndex < method->getRParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                writeNode(method->getBody(), indent);
                return;
            }
            case syntax::SyntaxType::GenericCallExpression: {
                auto call = std::static_pointer_cast<syntax::GenericCallExpression>(node);
                writeNode(call->getMethod(), indent);
                writeNode(call->getGenericArgList(), indent);


                while (_tokenIndex < call->getLParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                call->forEachArgument([&](auto arg, auto last) -> auto {
                    writeNode(arg.lock(), indent);
                    if (!last) {
                        _codeBuffer << ", ";
                        _tokenIndex++;
                    }
                });


                while (_tokenIndex < call->getRParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ")";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::CallExpression: {
                auto call = std::static_pointer_cast<syntax::CallExpression>(node);
                writeNode(call->getMethod(), indent);

                while (_tokenIndex < call->getLParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }

                _codeBuffer << "(";
                _tokenIndex++;

                call->forEachArgument([&](auto arg, auto last) -> auto {
                    writeNode(arg.lock(), indent);
                    if (!last) {
                        _codeBuffer << ", ";
                        _tokenIndex++;
                    }
                });

                while (_tokenIndex < call->getRParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }

                _codeBuffer << ")";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::ParamDeclaration: {
                auto param = std::static_pointer_cast<syntax::ParamDeclaration>(node);
                writeNode(param->getParamType().lock(), indent);
                _codeBuffer << " ";
                writeNode(param->getParamName().lock(), indent);
                if (param->getEqualIndex() != syntax::SyntaxNode::invalidTokenIndex) {
                    _codeBuffer << " = ";
                    _tokenIndex++;
                    while (_tokenIndex < param->getEqualIndex()) {
                        writeCurrentTokenAsComment(indent);
                    }
                    writeNode(param->getDefaultValue().lock(), indent);
                }
                break;
            }
            case syntax::SyntaxType::QualifiedName: {
                auto qualified = std::static_pointer_cast<syntax::QualifiedName>(node);
                writeNode(qualified->getLeft(), indent);


                while (_tokenIndex < qualified->getPeriodTokId()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ".";
                _tokenIndex++;
                writeNode(qualified->getRight(), indent);
                break;
            }
            case syntax::SyntaxType::ArrayType: {
                auto arrayType = std::static_pointer_cast<syntax::ArrayType>(node);
                writeNode(arrayType->getChildType(), indent);

                while (_tokenIndex < arrayType->getLSquareIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "[";
                _tokenIndex++;


                while (_tokenIndex < arrayType->getRSquareIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "]";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::BinaryExpression: {
                auto binaryExpr = std::static_pointer_cast<syntax::BinaryExpression>(node);
                writeNode(binaryExpr->getLeft(), indent);

                while (_tokenIndex < binaryExpr->getOpIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                std::string space = (binaryExpr->getOperatorType() == token::TokenType::op_period ? "" : " ");
                _codeBuffer << space
                            << token::TokenUtil::getSpell(binaryExpr->getOperatorType())
                            << space;
                _tokenIndex++;
                writeNode(binaryExpr->getRight(), indent);
                break;
            }
            case syntax::SyntaxType::ExplicitCastExpression: {
                auto castExpr = std::static_pointer_cast<syntax::ExplicitCastExpression>(node);

                while (_tokenIndex < castExpr->getLeftParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "(";
                _tokenIndex++;
                writeNode(castExpr->getTargetType(), indent);

                while (_tokenIndex < castExpr->getRightParenIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << ") ";
                _tokenIndex++;
                writeNode(castExpr->getOperand(), indent);
                break;
            }
            case syntax::SyntaxType::UnaryExpression: {
                auto unaryExpr = std::static_pointer_cast<syntax::UnaryExpression>(node);


                while (_tokenIndex < unaryExpr->getOpIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << token::TokenUtil::getSpell(unaryExpr->getOperatorType());
                _tokenIndex++;
                writeNode(unaryExpr->getRight(), indent);
                break;
            }
            case syntax::SyntaxType::IndexExpression: {
                auto indexExpr = std::static_pointer_cast<syntax::IndexExpression>(node);
                writeNode(indexExpr->getIndexedExpr(), indent);

                while (_tokenIndex < indexExpr->getLSquareIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "[";
                _tokenIndex++;
                writeNode(indexExpr->getIndex(), indent);

                while (_tokenIndex < indexExpr->getRSquareIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << "]";
                _tokenIndex++;
                break;
            }
            case syntax::SyntaxType::PostfixExpression: {
                auto postfixExpr = std::static_pointer_cast<syntax::PostfixExpression>(node);
                writeNode(postfixExpr->getOperand(), indent);
                while (_tokenIndex < postfixExpr->getOpIndex()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << token::TokenUtil::getSpell(postfixExpr->getOperatorType());
                _tokenIndex++;
                break;
            }

            case syntax::SyntaxType::StringLiteralExpression:
                _codeBuffer << '"' <<_context->tokens[_tokenIndex]->rawCode << '"';
                _tokenIndex++;
                break;
            case syntax::SyntaxType::ThisExpression:
            case syntax::SyntaxType::NumericLiteralExpression:
            case syntax::SyntaxType::InterlineComment:
            case syntax::SyntaxType::InlineComment:
            case syntax::SyntaxType::IdentifierName:
                while (_tokenIndex < node->start()) {
                    writeCurrentTokenAsComment(indent);
                }
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode;
                _tokenIndex++;
                break;
            case syntax::SyntaxType::NullExpression:
            case syntax::SyntaxType::UnknownType:
                throw "unexpected formatter internal error";
        }


    }

    void Formatter::writeIndent(std::size_t indent) {
        for (std::size_t i = 0; i < indent; ++i) {
            _codeBuffer << ' ';
        }
    }

    Formatter::Formatter(const std::shared_ptr<SyntaxContext> &context) {
        _context = context;
        _tokenIndex = 0;
    }

    void Formatter::writeCurrentTokenAsComment(std::size_t indent) {
        switch (_context->tokens[_tokenIndex]->type) {
            case token::TokenType::interline_comment:
            case token::TokenType::inline_comment:
                //_codeBuffer << std::endl;
                //writeIndent(indent);
                _codeBuffer << _context->tokens[_tokenIndex]->rawCode << std::endl;
                writeIndent(indent);
                _tokenIndex ++;
                break;
            default:
                throw "non-comment token was skipped";
        }
    }
}