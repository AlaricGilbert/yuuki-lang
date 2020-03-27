#include <yuuki/utils/string_util.h>
#include <cstring>

namespace yuuki::utils{

    bool StringUtil::unescape(const std::string &str, std::string &target, int &error_pos) {
        char *escaped = new char[str.length()];
        bool status = unescape(str.c_str(),escaped,error_pos,str.length());
        target = escaped;
        delete[] escaped;
        return status;
    }


    bool StringUtil::unescape(const char *str, char * &target, int &error_pos, int length = -1) {

        // Reference : https://github.com/yasuoka/unescape/blob/master/unescape.c
#define IS_XDIGIT(_c) (				\
	(('0' <= (_c) && (_c) <= '9')) ||	\
	(('a' <= (_c) && (_c) <= 'f')) ||	\
	(('A' <= (_c) && (_c) <= 'F')))
#define XDIGIT(_c) (						\
	(('0' <= (_c) && (_c) <= '9'))? (_c) - '0' :		\
	(('a' <= (_c) && (_c) <= 'f'))? (_c) - 'a' + 10 :	\
	(('A' <= (_c) && (_c) <= 'F'))? (_c) - 'A' + 10 : (-1))

        if(length<=0)
            length = std::strlen(str);
        char *out = new char[length];
        target = out;

        int i = 0, j = 0;
        bool escaped = false;
        for (; i < length; ++i) {
            if(!escaped){
                if(str[i]=='\\'){
                    escaped = true;
                } else{
                    out[j++] = str[i];
                }
            } else{
                escaped = false;
                switch (str[i]){
                    case 'a':
                        out[j++] = 7;
                        break;
                    case 'b':
                        out[j++] = 8;
                        break;
                    case 't':
                        out[j++] = 9;
                        break;
                    case 'n':
                        out[j++] = 10;
                        break;
                    case 'v':
                        out[j++] = 11;
                        break;
                    case 'f':
                        out[j++] = 12;
                        break;
                    case 'r':
                        out[j++] = 13;
                        break;
                    case '\\':
                    case '"':
                    case '\'':
                        out[j++] = str[i];
                    case '\n':
                        break;
                    case '0':{
                        out[j++] = 0;
                    }
                    case 'x':{
                        if((IS_XDIGIT(str[i+1]))&&(IS_XDIGIT(str[i+2]))){
                            out[j++] = (XDIGIT(str[i+1]) << 4) & (XDIGIT(str[i+2]));
                        }
                    }
                    default:
                        error_pos = i;
                        goto err;

                    case 'u':{
                        uint16_t W1,W2;
                        uint32_t U = 0;
                            if (!IS_XDIGIT(str[i + 1]) ||
                                !IS_XDIGIT(str[i + 2]) ||
                                !IS_XDIGIT(str[i + 3]) ||
                                !IS_XDIGIT(str[i + 4]))
                                goto err;
                        W1 =
                                (XDIGIT(str[i + 1]) << 12) |
                                (XDIGIT(str[i + 2]) << 8) |
                                (XDIGIT(str[i + 3]) << 4) |
                                XDIGIT(str[i + 4]);
                        i += 5;

                        if (W1 < 0xD800 || 0xDFFF < W1)
                            U = W1;
                        else if (!(0xD800 <= W1 && W1 <= 0xDBFF))
                            goto err;
                        else {
                            if (str[i] != '\\' ||
                                    str[i + 1] != 'u' ||
                                !IS_XDIGIT(str[i + 2]) ||
                                !IS_XDIGIT(str[i + 3]) ||
                                !IS_XDIGIT(str[i + 4]) ||
                                !IS_XDIGIT(str[i + 5]))
                                goto err;
                            W2 =
                                    (XDIGIT(str[i + 2]) << 12) |
                                    (XDIGIT(str[i + 3]) << 8) |
                                    (XDIGIT(str[i + 4]) << 4) |
                                    XDIGIT(str[i + 5]);
                            i += 6;

                            if (!(0xDC00 <= W2 && W2 <= 0xDFFF))
                                goto err;

                            U = ((W1 & 0x3FF) << 10) | (W2 & 0x3FF);
                            U += 0x10000;
                        }
                        if (U <= 0x0000007F)
                            out[j++] = U & 0x7F;
                        else if (U <= 0x000007FF) {
                            out[j++] = 0xC0 | ((U >> 6) & 0x1F);
                            out[j++] = 0x80 | (U & 0x3F);
                        } else if (U <= 0x0000FFFF) {
                            out[j++] = 0xE0 | ((U >> 12) & 0x0F);
                            out[j++] = 0x80 | ((U >> 6) & 0x3F);
                            out[j++] = 0x80 | (U & 0x3F);
                        } else if (0x00010000 <= U && U <= 0x0010FFFF) {
                            out[j++] = 0xF0 | ((U >> 18) & 0x07);
                            out[j++] = 0x80 | ((U >> 12) & 0x3F);
                            out[j++] = 0x80 | ((U >> 6) & 0x3F);
                            out[j++] = 0x80 | (U & 0x3F);
                        } else
                            goto err;
                        --i;
                    }

                }
            }
        }
        out[j++] = '\0';
        return true;
        err:
        error_pos = j;
        return false;
    }
}