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

        /**
         * Parse a full syntax unit and save it into given syntax context.
         */
        void parse();

        /**
         * Parse a qualified name or an identifier name.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::Name> parseName();

        /**
         * Parse an identifier name.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::IdentifierName> parseIdentifier();

        /**
         * Parse a class declaration, including recursively parse its nested sub-classes and members.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::ClassDeclaration> parseClass();

        /**
         * Parse a method declaration, including recursively parse its method body statements.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::MethodDeclaration> parseMethodDeclaration();

        /**
         * Parse a yuuki language valid type, including trivial type, generic type and array type.
         * @param errorRecover the recover target tokens should try to move to when error occurs
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::Type> parseType(const std::list<token::TokenType>& errorRecover = {});

        /**
         * Parse an expression by given precedence, will parse any matched situations that matches.
         * @param endTokens the tokens that when the expression parser met, just stop immediately
         * @param parentPrecedence the parent precedence, any expressions has same or lower precedence will be parsed
         * out immediately.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::Expression> parseExpression(const std::list<token::TokenType>& endTokens = {},
                                                            std::size_t parentPrecedence = token::OperatorUtil::initial);

        /**
         * Parse a field declaration, will parse their default values too.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::FieldDeclaration> parseFieldDeclaration();

        /**
         * Parse a statement, including flow-control statements, error-handling statements, expression statements and so on.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::Statement> parseStatement();

        /**
         * Parse a statement that constructed by an expression like self modify expressions and call expressions.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::ExpressionStatement> parseExpressionStatement();

        /**
         * Parse a statement that declares some local variables.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::VariableDeclarationStatement> parseVariableDeclarationStatement();

        /**
         * Parse a statement that constructed by a array of statements that packed by '{' and '}'.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::BlockStatement> parseBlockStatement();

        /**
         * Parse an if statement, with both if(expr) statement and if(expr) statement else statement situations.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::IfStatement> parseIfStatement();

        /**
         * Parse a while statement like while(expr) statement
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::WhileStatement> parseWhileStatement();

        /**
         * Parse a statement that made of a continue keyword and a semicolon.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::ContinueStatement> parseContinueStatement();

        /**
         * Parse a statement that made of a break keyword and a semicolon.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::BreakStatement> parseBreakStatement();

        /**
         * Parse a statement like return; or return expr;.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::ReturnStatement> parseReturnStatement();

        /**
         * Parse a statement like: goto label;.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::GotoStatement> parseGotoStatement();

        /**
         * Parse a label declare statement
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::LabelStatement> parseLabelStatement();

        /**
         * Parse a case statement, which made of a case-expression section and a case block statement.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::CaseStatement> parseCaseStatement();

        /**
         * Parse a statement that like case statement but should not accept any case expression
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::DefaultStatement> parseDefaultStatement();

        /**
         * Parse a switch statement that is almost a collection of case statements and default statment
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::SwitchStatement> parseSwitchStatement();

        /**
         * Parse a for statement, will just return if any error occurs.
         * @return parsed syntax node.
         */
        std::shared_ptr<syntax::ForStatement> parseForStatement();

        /**
         * Fill a param declarations, supporting default values.
         * @param list that should be filled.
         * @return whether the fill operation finished successfully.
         */
        bool fillParamList(const std::shared_ptr<syntax::ParamList>& list);

        /**
         * Fill a generic type declarations, supporting nested generic types.
         * @param list that should be filled.
         * @return whether the fill operation finished successfully.
         */
        bool fillGenericTypeList(const std::shared_ptr<syntax::GenericTypeList> &list);

        /**
         * Fill a modifier list that is a container of modifiers
         * @param list that should be filled.
         * @return whether the fill operation finished successfully.
         */
        bool fillModifierList(const std::shared_ptr<syntax::ModifierList>& list);

        /**
         * Skip over a name and return the result of the skipped name is valid.
         * @return whether we skipped successfully
         */
        bool skipOverAName();

        /**
         * Skip over a type declaration and return the result of the skipped name is valid.
         * @return whether we skipped successfully
         */
        bool skipOverATypeDeclaration();


        /**
         * Skip over a name and retuen the of the tested token arrays
         * @return whether we skipped successfully
         */
        bool skipOverAGenericArgument();

        /**
         * Skip over modifiers
         * @return whether we skipped successfully
         */
        bool skipOverAModifierList();

        /**
         * Split current multi-char operator to handle with misunderstanding between nested generic type declaration and
         * right shift operators like Type<T,List<T>> and a >> b.
         */
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
