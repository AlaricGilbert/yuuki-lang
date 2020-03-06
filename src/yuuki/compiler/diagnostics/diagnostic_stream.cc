#include <yuuki/compiler/diagnostics/diagnostic_stream.h>
namespace yuuki::compiler::diagnostics{

    DiagnosticStream::DiagnosticStream(const std::shared_ptr<feasy::SyntaxContextManager> &contextManager) {
        _contextManager = contextManager;
    }

    DiagnosticStream &DiagnosticStream::operator<<(std::unique_ptr<Diagnostic> diagPtr) {
        _diagnostics.push_back(std::move(diagPtr));
        return *this;
    }

    std::ostream &operator<<(std::ostream& ostream,const DiagnosticStream& diagnosticStream){
        for (int i = 0; i < diagnosticStream._diagnostics.size(); ++i) {
            diagnosticStream._diagnostics[i]->writeTo(ostream,diagnosticStream._contextManager);
        }
        return ostream;
    }

    std::shared_ptr<DiagnosticStream> operator<<(std::shared_ptr<DiagnosticStream>&streamPtr
            ,std::unique_ptr<Diagnostic> diagPtr){
        *streamPtr << std::move(diagPtr);
        return streamPtr;
    }
}