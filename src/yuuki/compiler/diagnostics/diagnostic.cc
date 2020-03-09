#include <yuuki/compiler/diagnostics/diagnostic.h>
#include <rang/rang.h>
#include <iostream>
namespace yuuki::compiler::diagnostics{
//    void Diagnostic::printErrors() {
//        for (auto &err:_errors) {
//            auto contextPtr = _contextManager->access(err.contextID);
//
//            size_t tokenLength = contextPtr->tokens[err.tokenIndex]->rawCode.length();
//
//            auto pos = contextPtr->locate(err.tokenIndex);
//            std::string path = contextPtr->codePath;
//
//            std::cout << path << ":" << pos.line << ":" << pos.offset << ": "
//                      << rang::fg::red << "error: " << rang::fg::reset << err.errorMessage << std::endl
//                      << contextPtr->lines[pos.line - 1] << std::endl;
//            if (err.inPlace) {
//                // get the position of the first char in the tokens
//                for (size_t sIdx = 0; sIdx < pos.offset; sIdx++) {
//                    std::cout << " ";
//                }
//                std::cout << rang::fg::green << "^";
//                // since tokens like interline-comments and string literals
//                // can take multi lines of code, to make sure that tildes have
//                // the right end position
//                size_t tildeNums = std::min(contextPtr->lines[pos.line - 1].length() - pos.offset,
//                                            tokenLength);
//                for (int i = 1; i < tildeNums; ++i) {
//                    std::cout << "~";
//                }
//            } else {
//                for (size_t sIdx = 0; sIdx < pos.offset + tokenLength; sIdx++) {
//                    std::cout << " ";
//                }
//                std::cout << rang::fg::green << "^";
//            }
//
//            std::cout << rang::fg::reset << std::endl;
//            if (!err.suggestionMessage.empty()) {
//                std::cout << err.suggestionMessage << std::endl;
//            }
//        }
//    }
//
//    void Diagnostic::errorAt(CompileError errorCode, size_t contextID, size_t tokenIndex, const std::string &errorMessage,
//                             const std::string &suggestionMessage) {
//        _errors.push_back(
//                DiagnosticErrorItem{
//                    .compileError = errorCode,
//                    .contextID = contextID,
//                    .tokenIndex = tokenIndex,
//                    .inPlace = true,
//                    .errorMessage = errorMessage,
//                    .suggestionMessage = suggestionMessage});
//    }
//
//    void Diagnostic::errorAfter(CompileError errorCode, size_t contextID, size_t tokenIndex, const std::string &errorMessage,
//                                const std::string &suggestionMessage) {
//        _errors.push_back(
//                DiagnosticErrorItem{
//                        .compileError = errorCode,
//                        .contextID = contextID,
//                        .tokenIndex = tokenIndex,
//                        .inPlace = false,
//                        .errorMessage = errorMessage,
//                        .suggestionMessage = suggestionMessage});
//    }
//
//
//    Diagnostic::Diagnostic(std::shared_ptr<compiler::feasy::SyntaxContextManager> &contextManager) {
//        this->_contextManager = contextManager;
//    }
    ErrorAtDiagnostic::ErrorAtDiagnostic(CompileError error, std::size_t contextID, std::size_t tokenID,
                                         const std::string &message, const std::string &suggestion) {
        _errorCode = error;
        _contextID = contextID;
        _tokenID = tokenID;
        _message = message;
        _suggestion = suggestion;
    }

    void
    ErrorAtDiagnostic::writeTo(std::ostream &stream, const std::shared_ptr<feasy::SyntaxContextManager> &ctxManager) {
        auto contextPtr = ctxManager->access(_contextID);

        size_t tokenLength = contextPtr->tokens[_tokenID]->rawCode.length();

        auto pos = contextPtr->locate(_tokenID);
        std::string path = contextPtr->codePath;

        auto isCout = stream.rdbuf() == std::cout.rdbuf();
        if (isCout) {
            // write to std::cout, we should make it colorful
            stream << path << ":" << pos.line << ":" << pos.offset << ": "
                   << rang::fg::red << "error: " << rang::fg::reset << _message << std::endl
                   << contextPtr->lines[pos.line - 1] << std::endl
                   // prepare for tip mark color
                   <<  rang::fg::green;
        } else {
            stream << path << ":" << pos.line << ":" << pos.offset << ": "
                   << "error: " << _message << std::endl
                   << contextPtr->lines[pos.line - 1] << std::endl;
        }

        // get the position of the first char in the tokens
        for (size_t sIdx = 0; sIdx < pos.offset; sIdx++) {
            stream << " ";
        }
        stream << "^";
        // since tokens like interline-comments and string literals
        // can take multi lines of code, to make sure that tildes have
        // the right end position
        size_t tildeNums = std::min(contextPtr->lines[pos.line - 1].length() - pos.offset,
                                    tokenLength);
        for (int i = 1; i < tildeNums; ++i) {
            stream << "~";
        }
        // reset color if it's write to std::cout
        if (isCout)
            stream << rang::fg::reset;
        stream << std::endl;
        if (!_suggestion.empty()) {
            stream << _suggestion << std::endl;
        }
    }

    ErrorAfterDiagnostic::ErrorAfterDiagnostic(CompileError error, std::size_t contextID, std::size_t tokenID,
                                               const std::string &message, const std::string &suggestion) {
        _errorCode = error;
        _contextID = contextID;
        _tokenID = tokenID;
        _message = message;
        _suggestion = suggestion;
    }

    void ErrorAfterDiagnostic::writeTo(std::ostream &stream,
                                       const std::shared_ptr<feasy::SyntaxContextManager> &ctxManager) {
        auto contextPtr = ctxManager->access(_contextID);

        size_t tokenLength = contextPtr->tokens[_tokenID]->rawCode.length();

        auto pos = contextPtr->locate(_tokenID);
        std::string path = contextPtr->codePath;

        auto isCout = stream.rdbuf() == std::cout.rdbuf();

        if (isCout) {
            // write to std::cout, we should make it colorful
            stream << path << ":" << pos.line << ":" << pos.offset << ": "
                   << rang::fg::red << "error: " << rang::fg::reset << _message << std::endl
                   << contextPtr->lines[pos.line - 1] << std::endl
                   // prepare for tip mark color
                   <<  rang::fg::green;
        } else {
            stream << path << ":" << pos.line << ":" << pos.offset << ": "
                   << "error: " << _message << std::endl
                   << contextPtr->lines[pos.line - 1] << std::endl;
        }

        for (size_t sIdx = 0; sIdx < pos.offset + tokenLength; sIdx++) {
            stream << " ";
        }
        stream << "^";
        // reset color if it's write to std::cout
        if (isCout)
            stream << rang::fg::reset;
        stream << std::endl;
        if (!_suggestion.empty()) {
            stream << _suggestion << std::endl;
        }
    }

    ErrorBeforeDiagnostic::ErrorBeforeDiagnostic(CompileError error, std::size_t contextID, std::size_t tokenID,
                                                 const std::string &message, const std::string &suggestion) {
        _errorCode = error;
        _contextID = contextID;
        _tokenID = tokenID;
        _message = message;
        _suggestion = suggestion;
    }

    void ErrorBeforeDiagnostic::writeTo(std::ostream &stream,
                                        const std::shared_ptr<feasy::SyntaxContextManager> &ctxManager) {
        auto contextPtr = ctxManager->access(_contextID);

        size_t tokenLength = contextPtr->tokens[_tokenID]->rawCode.length();

        auto pos = contextPtr->locate(_tokenID);
        std::string path = contextPtr->codePath;

        auto isCout = stream.rdbuf() == std::cout.rdbuf();

        if (isCout) {
            // write to std::cout, we should make it colorful
            stream << path << ":" << pos.line << ":" << pos.offset << ": "
                   << rang::fg::red << "error: " << rang::fg::reset << _message << std::endl
                   << contextPtr->lines[pos.line - 1] << std::endl
                   // prepare for tip mark color
                   <<  rang::fg::green;
        } else {
            stream << path << ":" << pos.line << ":" << pos.offset << ": "
                   << "error: " << _message << std::endl
                   << contextPtr->lines[pos.line - 1] << std::endl;
        }

        for (size_t sIdx = 0; sIdx < pos.offset - 1; sIdx++) {
            stream << " ";
        }
        stream << "^";
        // reset color if it's write to std::cout
        if (isCout)
            stream << rang::fg::reset;
        stream << std::endl;
        if (!_suggestion.empty()) {
            stream << _suggestion << std::endl;
        }
    }
}
