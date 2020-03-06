#ifndef YUUKI_DIAGNOSTIC_H
#define YUUKI_DIAGNOSTIC_H

#include <yuuki/compiler/diagnostics/compile_errors.h>
#include <yuuki/compiler/feasy/syntax_context_manager.h>
#include <yuuki/compiler/feasy/syntax_context.h>
#include <string>
#include <vector>
#include <ostream>
#include <memory>
namespace yuuki::compiler::diagnostics{
    class Diagnostic{
    public:
        virtual void writeTo(std::ostream& stream,const std::shared_ptr<feasy::SyntaxContextManager>& ctxManager) = 0;
        virtual ~Diagnostic() = default;
    };

    class ErrorAtDiagnostic: public Diagnostic{
    public:
        ErrorAtDiagnostic(CompileError error, std::size_t contextID, std::size_t tokenID,
                          const std::string& message,const std::string& suggestion);
        void writeTo(std::ostream& stream,const std::shared_ptr<feasy::SyntaxContextManager>& ctxManager) override ;
    private:
        CompileError _errorCode;
        std::size_t _contextID;
        std::size_t _tokenID;
        std::string _message;
        std::string _suggestion;
    };

    class ErrorAfterDiagnostic: public Diagnostic{
    public:
        ErrorAfterDiagnostic(CompileError error, std::size_t contextID, std::size_t tokenID,
                             const std::string& message,const std::string& suggestion);
        void writeTo(std::ostream& stream,const std::shared_ptr<feasy::SyntaxContextManager>& ctxManager) override ;
    private:
        CompileError _errorCode;
        std::size_t _contextID;
        std::size_t _tokenID;
        std::string _message;
        std::string _suggestion;
    };

    class ErrorRangesDiagnostic: public Diagnostic{

    };

//
//    /**
//     * A class that provides diagnostic information collect and report service.
//     */
//    class Diagnostic{
//    public:
//        /**
//         * Construct a diagnostic collector instance by given syntax context manager;
//         * @param contextManager
//         */
//        explicit Diagnostic(std::shared_ptr<feasy::SyntaxContextManager> &contextManager);
//        /**
//         * Push an compile error which happens to the given token position into the errors array.
//         * @param errorCode
//         * @param contextID
//         * @param tokenIndex
//         * @param errorMessage
//         * @param suggestionMessage
//         */
//        void errorAt(CompileError errorCode, size_t contextID, size_t tokenIndex,const std::string& errorMessage,
//                const std::string& suggestionMessage = "");
//        /**
//         * Push an compile error which happens after the given token position into the errors array.
//         * @param errorCode
//         * @param contextID
//         * @param tokenIndex
//         * @param errorMessage
//         * @param suggestionMessage
//         */
//        void errorAfter(CompileError errorCode, size_t contextID, size_t tokenIndex,const std::string& errorMessage,
//                const std::string& suggestionMessage = "");
//        /**
//         * Print the errors into the standard output stream (by std::cout)
//         */
//        void printErrors();
//        /**
//         * A struct that used to store compile errors.
//         */
//        struct DiagnosticErrorItem{
//            CompileError compileError;
//            size_t       contextID;
//            size_t       tokenIndex;
//            bool         inPlace;
//            std::string  errorMessage;
//            std::string  suggestionMessage;
//        };
//    private:
//        // error list
//        std::vector<DiagnosticErrorItem> _errors;
//        // copy of context manager
//        std::shared_ptr<compiler::feasy::SyntaxContextManager> _contextManager;
//    };
}
#endif //YUUKI_DIAGNOSTIC_H
