#ifndef YUUKI_CODE_POSTION_H
#define YUUKI_CODE_POSTION_H

#include <cstddef>
namespace yuuki::compiler::feasy{
    /**
     * Represents a position in the code.
     */
    struct CodePosition{
        std::size_t line;
        std::size_t offset;
    };
}
#endif //YUUKI_CODE_POSTION_H
