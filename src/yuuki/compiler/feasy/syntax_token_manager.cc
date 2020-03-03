#include <yuuki/compiler/feasy/syntax_context_manager.h>
namespace yuuki::compiler::feasy{
    std::shared_ptr<SyntaxContext> SyntaxContextManager::create(const std::string_view& code) {
        auto context = std::make_shared<SyntaxContext>(code,_contexts.size());
        _contexts.push_back(context);
        return context;
    }
    std::shared_ptr<SyntaxContext> SyntaxContextManager::access(std::size_t index) {
        if(index < _contexts.size())
            return _contexts[index];
        return nullptr;
    }

    std::shared_ptr<SyntaxContext> SyntaxContextManager::operator[](std::size_t index) {
        if(index < _contexts.size())
            return _contexts[index];
        return nullptr;
    }
}