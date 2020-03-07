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
    /**
     * Abstract class that defines a diagnostic should implements.
     */
    class Diagnostic{
    public:
        virtual void writeTo(std::ostream& stream,const std::shared_ptr<feasy::SyntaxContextManager>& ctxManager) = 0;
        virtual ~Diagnostic() = default;
    };

    /**
     * A diagnostic that issues error message at the position of the specified token.
     */
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

    /**
     * A diagnostic that issues error message after the position of the specified token.
     */
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

    /**
     * A diagnostic that issues error message between the tokens in the range
     */
    class ErrorRangesDiagnostic: public Diagnostic{

    };
}
#endif //YUUKI_DIAGNOSTIC_H
