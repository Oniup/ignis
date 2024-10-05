#ifndef IGNIS_CORE_CSTR_H
#define IGNIS_CORE_CSTR_H

#define CSTR_LEN(cstr)                                                         \
    ({                                                                         \
        usize len = 0;                                                         \
        if (cstr) {                                                            \
            char c = cstr[0];                                                  \
            while (c != '\0') {                                                \
                len++;                                                         \
                c = cstr[len];                                                 \
            }                                                                  \
        }                                                                      \
        len;                                                                   \
    })

#define CSTR_CMP(cstr1, cstr2)                                                 \
    ({                                                                         \
        b8    res  = true;                                                     \
        usize len1 = CSTR_LEN(cstr1);                                          \
        usize len2 = CSTR_LEN(cstr2);                                          \
        if (len1 == len2) {                                                    \
            for (usize i = 0; i < len1; i++) {                                 \
                if (cstr1[i] != cstr2[i]) {                                    \
                    res = false;                                               \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
        res;                                                                   \
    })

#endif
