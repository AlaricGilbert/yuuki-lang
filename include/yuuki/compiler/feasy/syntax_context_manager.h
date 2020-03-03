#ifndef YUUKI_SYNTAX_CONTEXT_MANAGER_H
#define YUUKI_SYNTAX_CONTEXT_MANAGER_H

#include <yuuki/compiler/feasy/syntax_context.h>
namespace yuuki::compiler::feasy{
    /**
     * A SyntaxContext manager: which is going to be used in multiple file compilations.
     */
    class SyntaxContextManager{
    public:
        /**
         * Create a SyntaxContext by given code reference string
         * @param code
         * @return
         */
        std::shared_ptr<SyntaxContext> create(const std::string_view& code);
        /**
         * Access to the SyntaxContext with the given index
         * @param index
         * @return
         */
        std::shared_ptr<SyntaxContext> access(std::size_t index);

        /**
         * Access to the SyntaxContext with the given index
         * @param index
         * @return
         */
        std::shared_ptr<SyntaxContext> operator[](std::size_t index);

    private:
        std::vector<std::shared_ptr<SyntaxContext>> _contexts;
    };
}
#endif //YUUKI_SYNTAX_CONTEXT_MANAGER_H
