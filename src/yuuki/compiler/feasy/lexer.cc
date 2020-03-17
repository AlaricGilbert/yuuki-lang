#include "yuuki/compiler/feasy/lexer.h"
#include "yuuki/compiler/feasy/token/token_util.h"
#include <yuuki/compiler/diagnostics/diagnostic_builder.h>

namespace yuuki::compiler::feasy{

    using namespace diagnostics;
    using namespace token;
    Lexer::Lexer(const std::shared_ptr<SyntaxContext> &context,
            const std::shared_ptr<diagnostics::DiagnosticStream> &diagnostiStream) {
        _context = context;
        _diagnosticStream = diagnostiStream;
        _code = context->code;
        _position = 0;
        _eof = false;
    }

    bool Lexer::lexToken() {
        lex_start:

        if (_eof)
            return true;

        // returns a eof token when lexer reaches the end of the code string_view
        if (_position >= _code.size()) {
            auto token = std::make_unique<token::Token>();
            token->type = token::TokenType::eof;
            token->offset = _position;
            token->rawCode = _code.substr(_position, 1);
            _context->tokens.push_back(std::move(token));
            _eof = true;
            return true;
        }

        // skip space characters
        if (std::isspace(_code[_position])) {
            _position++;
            // goto start to lex start from next token to skip spaces
            goto lex_start;
        }

        // lex number const
        if (('0' <= _code[_position]) && (_code[_position] <= '9')) {
            return lexNumericConst();
        }

        // lex string const
        if (_code[_position] == '\"') {
            return lexStringConst();
        }

        // lex comments
        if (_code[_position] == '/' && (_position + 1 < _code.size())) {
            switch (_code[_position + 1]) {
                // inline comment
                case '/':
                    return lexInlineComment();
                    // interline comment
                case '*':
                    return lexInterlineComment();
                default:
                    break;
            }

        }

        // lex punctuator
        if (token::TokenUtil::isPunctuator(token::TokenUtil::getType(_code[_position]))) {
            auto token = std::make_unique<token::Token>();
            token->type = token::TokenUtil::getType(_code[_position]);
            token->rawCode = _code.substr(_position, 1);
            token->offset = _position;
            _context->tokens.push_back(std::move(token));
            _position++;
            return true;
        }

        // lex operator
        if (token::TokenUtil::isOperator(token::TokenUtil::getType(_code[_position]))) {
            return lexOperator();
        }

        // lex normal identifier
        if ((_code[_position] == '_') ||
            (('A' <= _code[_position]) && (_code[_position] <= 'Z')) ||
            (('a' <= _code[_position]) && (_code[_position] <= 'z')))
            return lexIdentifier();

        // unicode identifier
        if ((((unsigned char) _code[_position]) > 127) && isUnicodeCharValidOfIdentifier())
            return lexIdentifier();

        // neither of the situation is satisfied return error

        auto token = std::make_unique<token::Token>();
        token->type = token::TokenType::unknown;
        token->offset = _position;
        while (!isCurrCharSplitter())
            _position++;
        token->rawCode = _code.substr(_position, _position - token->offset - 1);
        _context->tokens.push_back(std::move(token));
        // push error
        _diagnosticStream << DiagnosticBuilder::
        error(CompileError::UnexpectedChars, _context->syntaxID)
                            .after(_context->tokens.size()-1)
                            .message("unexpected characters")
                            .build();
        return false;
    }

    bool Lexer::lexStringConst() {
        auto token = std::make_unique<token::Token>();
        token->offset = _position;
        token->type = token::TokenType::string_const;
        size_t startPos = _position;

        bool escaped = false;
        // skip the '"'
        _position++;
        // before we met the end of the file
        while (_position < _code.size()){
            // get the current char, handle it with a DFA
            switch (_code[_position]){
                // we met a new '"'
                case '\"':{
                    // if the quote mark is not escaped, marks the string ended
                    if(!escaped){
                        // form a token info and return
                        token->rawCode = _code.substr(startPos + 1, _position - startPos - 1);
                        _context->tokens.push_back(std::move(token));
                        // skip the close '"'
                        _position++;
                        return true;
                    }
                    // if th quote mark is escaped, we treat it like other chars and continue
                    escaped = false;
                    break;
                }
                // we met the escape mark '\'
                case '\\':{
                    // mark the escape mode negative to itself
                    // true: when the current char escaped, it means current '\' is a escaped char so there
                    //       have already a char escaped, we should set it to false;
                    // false:when the current char not escaped, it means current '\' should be treated as a
                    //       mark of next char escaping, we should set it to true.
                    escaped = !escaped;
                    break;
                }
                // we met the new-line code
                case '\n':{
                    // when the new-line symbol is not escaped
                    if(!escaped){
                        // it means it's a end of string that didn't end
                        // form the result and push an error
                        token->rawCode = _code.substr(startPos,_position - startPos);
                        _context->tokens.push_back(std::move(token));
                        _position++;
                        _diagnosticStream << DiagnosticBuilder::
                        error(CompileError::StringNotClosed, _context->syntaxID)
                                            .after(_context->tokens.size()-1)
                                            .message("string not closed")
                                            .build();
                        return false;
                    }
                    // when the new-line symbol is escaped, set escaped variable to false
                    escaped = false;
                    break;
                }
                default:{
                    escaped = false;
                    break;
                }
            }
            _position++;
        }
        // we met the end of the code, but the mark ends the string still not found
        // instantly form the token result and push an error to the diagnostic
        token->rawCode = _code.substr(startPos,_position - startPos);
        _context->tokens.push_back(std::move(token));
        _diagnosticStream << DiagnosticBuilder::
        error(CompileError::StringNotClosed, _context->syntaxID)
                            .after(_context->tokens.size()-1)
                            .message("string not closed")
                            .build();
        return false;
    }

    bool Lexer::lexNumericConst() {
        auto token = std::make_unique<token::Token>();
        token->type = token::TokenType::numeric_const;
        token->offset = _position;

        // there are few types of numeric token
        // we can do some basic valid check for it
        enum NumericType{
            NORMAL,
            FLOAT,
            SCIENTIFIC,
            OCT,
            HEX
        };
        // default type is NORMAL
        NumericType type = NORMAL;
        size_t startPos = _position;
        // initial judgements:
        // numbers start with 0
        if(_code[_position] == '0') {
            if (_position + 1 < _code.size()) {
                switch (_code[_position + 1]) {
                    // for '0.123' type, it's a float number
                    case '.':
                        type = FLOAT;
                        _position += 2;
                        break;
                    // for hex numbers like '0x88ff'
                    case 'x':
                    case 'X':
                        type = HEX;
                        _position += 2;
                        break;
                    // for octane numbers like '012'
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        type = OCT;
                        _position += 1;
                        break;
                    // other situations should be treated as an error
                    default:
                        // find the end of the current token
                        while (!isCurrCharSplitter())
                            _position++;
                        // form the result and push the error to the diagnostic info
                        token->rawCode = _code.substr(_position, _position - startPos - 1);
                        _context->tokens.push_back(std::move(token));
                        _diagnosticStream << DiagnosticBuilder::
                        error(CompileError::InvalidNumericConst, _context->syntaxID)
                                            .at(_context->tokens.size()-1)
                                            .message("invalid numeric const")
                                            .build();
                        return false;
                }
            }
        }
        continue_lex:
        // real numeric token lex DFA
        switch (_code[_position]){
            // we met a period token
            case '.':{
                // if it's a normal number
                if(type == NORMAL){
                    // set it's a float
                    type = FLOAT;
                    _position++;
                    goto continue_lex;
                }
                // number token ended
                goto form_result;
            }
            case '0':case '1':case '2':case '3':case '4':
            case '5':case '6':case '7':{
                _position++;
                goto continue_lex;
            }
            // we should handle special situations for octane numbers
            case '8':case '9':{
                if(type == OCT){
                    // '8' and '9' should be treated as an end sign of a octane number
                    goto form_result;
                }
                _position++;
                goto continue_lex;
            }
            // we should handle special situations for hex numbers
            case 'a':case 'A':case 'b':case 'B':case 'c':case 'C':
            case 'd':case 'D':case 'f':case 'F':{
                // these letters are treated as numbers in hex mode
                if(type == HEX){
                    _position++;
                    goto continue_lex;
                }
                // we should just end the lex of a number when we met a
                // letter that afters a non-hex number
                goto form_result;
            }
            case 'e':
            case 'E':{
                // scientific write mode of a number
                // like "123e-7" which means 123 * math.pow(10,-7)
                if(type == FLOAT || type == NORMAL) {
                    if ((_position + 1 < _code.size()) &&
                        (((_code[_position + 1] == '+')) || ((_code[_position + 1] == '-'))))
                        _position++;
                    _position++;
                    type = SCIENTIFIC;
                    goto continue_lex;
                } else if (type == HEX){
                    // 'e' and 'E' are treated as number members in hex mode
                    _position++;
                    goto continue_lex;
                } else{
                    // we should just end the lex of a number when we met a
                    // letter that afters a non-hex number
                    goto form_result;
                }
            }
            default:
                // end the number token when met other unexpected chars
                goto form_result;
        }
        form_result:
        token->rawCode = _code.substr(startPos,_position - startPos);
        _context->tokens.push_back(std::move(token));
        return true;
    }

    bool Lexer::lexIdentifier(size_t offset) {
        if (_code.size() > _position + offset) {
            // normal identifier
            if ((_code[_position + offset] == '_') ||
                (('A' <= _code[_position + offset]) && (_code[_position + offset] <= 'Z')) ||
                (('a' <= _code[_position + offset]) && (_code[_position + offset] <= 'z')) ||
                (('0' <= _code[_position + offset]) && (_code[_position + offset] <= '9')))
                return lexIdentifier(offset + 1);
            // unicode identifier
            if ((((unsigned char)_code[_position + offset]) > 127) && isUnicodeCharValidOfIdentifier())
                return lexIdentifier(offset + 1);
        }
        auto token = std::make_unique<token::Token>();
        token->offset = _position;
        token->rawCode = _code.substr(_position, offset);

        // get if the identifier have already been a keyword of yuuki language
        token::TokenType type = token::TokenUtil::getType(std::string(token->rawCode));
        token->type = type == token::TokenType::unknown ? token::TokenType::identifier : type;
        _position += offset;
        _context->tokens.push_back(std::move(token));
        return true;
    }

    bool Lexer::lexOperator() {
        size_t length = 1;
        // check for multi char operators
        while (token::TokenUtil::isOperator(
                token::TokenUtil::getType(
                        (std::string) _code.substr(_position,length)))
                        && length + _position <= _code.size()){
            length++;
        }
        length -= 1;

        auto token = std::make_unique<token::Token>();
        token->offset = _position;
        token->rawCode = _code.substr(_position,length);
        // get the type of the operator
        token->type = token::TokenUtil::getType((std::string) token->rawCode);
        _context->tokens.push_back(std::move(token));
        _position += length;
        return true;
    }

    bool Lexer::lexInlineComment() {
        // handle with inline comments
        size_t startPos = _position;

        auto token = std::make_unique<token::Token>();
        token->type = token::TokenType::inline_comment;
        token->offset = startPos;
        // when we haven't met line switch before end of file
        while (_position < _code.size()) {
            _position++;
            // ends the token with a new-line symbol
            if (_code[_position] == '\n') {
                break;
            }
        }
        // form the token and return
        token->rawCode = _code.substr(startPos, _position - startPos);
        _context->tokens.push_back(std::move(token));
        return true;
    }

    bool Lexer::lexInterlineComment() {
        size_t startPos = _position;
        auto token = std::make_unique<token::Token>();
        token->type = token::TokenType::interline_comment;
        token->offset = startPos;
        _position += 2;
        // when we haven't met the char before the last char
        while (_position < _code.size() - 1) {
            // check if it satisfies the "*/" which is the end of interline comment
            if ((_code[_position] == '*') && (_code[_position + 1] == '/')) {
                // form the result and return
                token->rawCode = _code.substr(startPos, _position + 2 - startPos);
                _context->tokens.push_back(std::move(token));
                _position+=2;
                return true;
            }
            _position++;
        }
        // we are going to met the eof but we haven't met the end of the symbol to end the interline comment
        // move to the end contain the last char
        _position += 1;
        // form the result and push the error to the diagnostic
        token->rawCode = _code.substr(startPos, _position - startPos);
        _context->tokens.push_back(std::move(token));
        _diagnosticStream << DiagnosticBuilder::
        error(CompileError::InterlineCommentNotClosed, _context->syntaxID)
                            .at(_context->tokens.size()-1)
                            .message("interline comment not closed")
                            .build();
        return false;
    }

    bool Lexer::isUnicodeCharValidOfIdentifier() {
        // FIXME: unicode chars are not validated yet!
        return true;
    }

    bool Lexer::isCurrCharSplitter() {
        if(_code[_position]>127)
            return false;
        switch (_code[_position]){
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
            case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D':
            case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
            case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
            case 'Y': case 'Z': case '_':
                return false;
        }
        return true;
    }

    bool Lexer::lex() {
        bool successful = true;
        do {
            if (!lexToken())
                successful = false;
        } while (!_eof);
        return successful;
    }

}