#ifndef YUUKI_PARSER_H
#define YUUKI_PARSER_H

#include <memory>
#include <list>
#include <yuuki/compiler/diagnostics/diagnostic_stream.h>
#include <yuuki/compiler/feasy/syntax_context.h>
#include <yuuki/compiler/feasy/syntax/binary_expression.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/namespace_declaration.h>
#include <yuuki/compiler/feasy/syntax/unary_expression.h>
#include <yuuki/compiler/feasy/token/operator_util.h>
#include <yuuki/compiler/feasy/syntax/field_declaration.h>
/**
 * Parser [WIP]
 *   To accelerate the development speed and make functions flavour unify, temporarily add a basic rule as a notification:
 *   Parser Basic Rule:
 *     1. Any sub-parse function should accept a token which marks itself, and end with its end
 *        which means if the SyntaxNode::start() and SyntaxNode::end() fully implemented,
 *        result of any parse function should generate a std::shared_ptr<SyntaxNode> instance i,
 *        which has i->start() equals the value of _tokenIndex when function called and has i->
 *        end() equals the value of _tokenIndex when function returned
 *        To achieve the goal, we should develop a set of 'look ahead' tool functions to judge the
 *        type of the syntax to be parsed.
 *     2. movements code, 'see forward' and universal error recovery code should be abstracted to tool
 *        functions like move() or jumpOverComments()
 *     3. tool functions should be inline
 */
namespace yuuki::compiler::feasy {
    /**
     * Syntax analyser for yuuki language.
     */
    class Parser {
    public:
        /**
         * Constructs a lexer from given syntax analyse context and diagnostic info reporter.
         * @param context:  the compile context during lexical analyse and syntax analyse
         * @param diagnosticStream: the diagnostic info collector to report errors to
         */
        Parser(const std::shared_ptr<SyntaxContext> &context,
               const std::shared_ptr<diagnostics::DiagnosticStream> &diagStream);

        void parse();

        std::shared_ptr<syntax::Name> parseName();

        std::shared_ptr<syntax::IdentifierName> parseIdentifier();

        std::shared_ptr<syntax::ClassDeclaration> parseClass();

        std::shared_ptr<syntax::MethodDeclaration> parseMethodDeclaration();

        std::shared_ptr<syntax::Type> parseType(const std::list<token::TokenType>& errorRecover = {});

        std::shared_ptr<syntax::Expression> parseExpression(const std::list<token::TokenType>& endTokens = {},
                                                            std::size_t parentPrecedence = token::OperatorUtil::initial);

        std::shared_ptr<syntax::FieldDeclaration> parseFieldDeclaration();

        std::shared_ptr<syntax::Statement> parseStatement();

        std::shared_ptr<syntax::ExpressionStatement> parseExpressionStatement();

        std::shared_ptr<syntax::VariableDeclarationStatement> parseVariableDeclarationStatement();

        std::shared_ptr<syntax::BlockStatement> parseBlockStatement();

        std::shared_ptr<syntax::IfStatement> parseIfStatement();

        std::shared_ptr<syntax::WhileStatement> parseWhileStatement();

        std::shared_ptr<syntax::ContinueStatement> parseContinueStatement();

        std::shared_ptr<syntax::BreakStatement> parseBreakStatement();

        std::shared_ptr<syntax::ReturnStatement> parseReturnStatement();

        std::shared_ptr<syntax::GotoStatement> parseGotoStatement();

        std::shared_ptr<syntax::LabelStatement> parseLabelStatement();

        std::shared_ptr<syntax::CaseStatement> parseCaseStatement();

        std::shared_ptr<syntax::DefaultStatement> parseDefaultStatement();

        std::shared_ptr<syntax::SwitchStatement> parseSwitchStatement();

        std::shared_ptr<syntax::ForStatement> parseForStatement();

        bool fillParamList(const std::shared_ptr<syntax::ParamList>& list);
        bool fillGenericTypeList(const std::shared_ptr<syntax::GenericTypeList> &list);
        bool fillModifierList(const std::shared_ptr<syntax::ModifierList>& list);
        bool skipOverAName();
        bool skipOverATypeDeclaration();
        bool skipOverAGenericArgument();
        bool skipOverAModifierList();
        void splitCurrentMultiCharOperator();


    private:

        inline void move(std::list<token::TokenType> acceptableEndTokType) {
            token::TokenType lastType;
            do {
                lastType = _context->tokens[_tokenIndex++]->type;
            } while ((std::find(acceptableEndTokType.begin(), acceptableEndTokType.end(), lastType) ==
                      acceptableEndTokType.end())
                     && (lastType != token::TokenType::eof));
            _tokenIndex--;
        }

        inline void jumpOverComments() {
            token::TokenType lastType;
            do {
                lastType = _context->tokens[_tokenIndex++]->type;
            } while (
                    (lastType == token::TokenType::interline_comment || lastType == token::TokenType::inline_comment) &&
                    _tokenIndex < _context->tokens.size());
            _tokenIndex--;
        }

        inline void moveAndJumpOverComments(){
            token::TokenType lastType;
            do {
                lastType = _context->tokens[++_tokenIndex]->type;
            } while (
                    (lastType == token::TokenType::interline_comment || lastType == token::TokenType::inline_comment) &&
                    _tokenIndex < _context->tokens.size());
        }

        inline token::TokenType getCurrentTokenType() {
            return _context->tokens[_tokenIndex]->type;
        }

        inline token::TokenType getTokenType(std::size_t index){
            return _context->tokens[index]->type;
        }

        inline token::TokenType peekTokenType(std::size_t offset) {
            return _context->tokens[_tokenIndex + offset]->type;
        }

        inline std::size_t getFirstNotComment() {
            std::size_t pos = _tokenIndex;
            while (getTokenType(pos) == token::TokenType::inline_comment ||
                   getTokenType(pos) == token::TokenType::interline_comment)
                pos++;
            return pos;
        }

        inline std::size_t getNextNotComment() {
            if(_tokenIndex >= _context->tokens.size() - 1)
                return _tokenIndex;
            std::size_t pos = _tokenIndex + 1;
            while (getTokenType(pos) == token::TokenType::inline_comment ||
                   getTokenType(pos) == token::TokenType::interline_comment)
                pos++;
            return pos;
        }

        // copy of the context
        std::shared_ptr<SyntaxContext> _context;
        // copy of the diagnostic info collector
        std::shared_ptr<diagnostics::DiagnosticStream> _diagnosticStream;
        // the index of the current token
        size_t _tokenIndex;
    };
}
#endif //YUUKI_PARSER_H
