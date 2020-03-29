#ifndef YUUKI_FORMATTER_H
#define YUUKI_FORMATTER_H

#include <yuuki/compiler/feasy/syntax_context.h>
#include <string>
#include <sstream>
namespace yuuki::compiler::feasy{
    class Formatter{
    public:
        explicit Formatter(const std::shared_ptr<SyntaxContext>& context);
        std::string format();
    private:
        void writeNode(const std::shared_ptr<syntax::SyntaxNode>&node, std::size_t indent);
        void writeCurrentTokenAsComment(std::size_t indent);
        void writeIndent(std::size_t indent);

        std::shared_ptr<SyntaxContext> _context;
        std::stringstream _codeBuffer;
        std::size_t _tokenIndex;
    };
}
#endif //YUUKI_FORMATTER_H
