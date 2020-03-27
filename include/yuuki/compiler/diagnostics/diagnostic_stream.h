#ifndef YUUKI_DIAGNOSTIC_STREAM_H
#define YUUKI_DIAGNOSTIC_STREAM_H

#include <yuuki/compiler/diagnostics/diagnostic.h>
#include <yuuki/compiler/feasy/syntax_context_manager.h>
#include <vector>
#include <memory>

namespace yuuki::compiler::diagnostics{
    class DiagnosticStream{
    public:
        explicit DiagnosticStream(const std::shared_ptr<feasy::SyntaxContextManager> &contextManager);
        DiagnosticStream& operator<<(std::unique_ptr<Diagnostic> diagPtr);
        friend std::ostream &operator<<(std::ostream& ostream,const DiagnosticStream& diagnosticStream);
        void clear();
    private:
        std::shared_ptr<feasy::SyntaxContextManager> _contextManager;
        std::vector<std::unique_ptr<Diagnostic>> _diagnostics;
    };
    std::shared_ptr<DiagnosticStream> operator<<(std::shared_ptr<DiagnosticStream>&streamPtr
            ,std::unique_ptr<Diagnostic> diagPtr);
}
#endif //YUUKI_DIAGNOSTIC_STREAM_H
