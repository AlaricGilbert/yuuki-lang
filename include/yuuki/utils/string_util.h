#ifndef YUUKI_STRING_UTIL_H
#define YUUKI_STRING_UTIL_H

#include <string>
#include "yuuki/utils/string_util.h"

namespace yuuki::utils {
    /**
     * A utility class provides some string operations.
     */
    class StringUtil {
    public:
        /**
         * Unescape a string.
         * @param str
         * @param target
         * @param error_pos
         * @return
         */
        static bool unescape(const std::string &str, std::string &target, int &error_pos);

        /**
         * Unescape a string.
         * @param str
         * @param target
         * @param error_pos
         * @param length
         * @return
         */
        static bool unescape(const char *str, char *&target, int &error_pos, int length);

    private:
        StringUtil() = default;
    };
}
#endif //YUUKI_STRING_UTIL_H
