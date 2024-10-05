#ifndef IGNIS_CORE_DYARR_H
#define IGNIS_CORE_DYARR_H

#include "ignis/core/defines.h"

typedef struct dyarr_header {
    usize len;
    usize cap;
    i8    stride;
} dyarr_header_t;

#define DYARR(_type)       _type*
#define DYARR_HEADER(_arr) ((dyarr_header_t*)_arr - 1)
#define DYARR_DATA(_hdr)   (_hdr + 1)

#define dyarr_len(_arr)    DYARR_HEADER(_arr)->len
#define dyarr_cap(_arr)    DYARR_HEADER(_arr)->cap
#define dyarr_stride(_arr) DYARR_HEADER(_arr)->stride

void* _dyarr_create(usize stride, usize size, usize cap);
void  dyarr_destroy(void* arr);

#define dyarr_create(_type, _cap) _dyarr_create(sizeof(_type), 0, _cap)
#define dyarr_create_empty(_type) _dyarr_create(sizeof(_type), 0, 0)
#define dyarr_create_size(_type, _size, _cap)                                  \
    _dyarr_create(sizeof(_type), _size, _cap)

void* _dyarr_resize(void* arr, usize size);
void* _dyarr_realloc(void* arr, usize cap);

#define dyarr_resize(_arr, _size)                                              \
    ({                                                                         \
        if (_arr) {                                                            \
            _arr = _dyarr_resize(_arr, _size);                                 \
        } else {                                                               \
            _arr = _dyarr_create(sizeof(*_arr), _size, _size);                 \
        }                                                                      \
    })

#define dyarr_realloc(_arr, _cap)                                              \
    ({                                                                         \
        if (_arr) {                                                            \
            _arr = _dyarr_realloc(_arr, _cap);                                 \
        } else {                                                               \
            _arr = _dyarr_create(sizeof(*_arr), 0, _cap);                      \
        }                                                                      \
    })

void* _dyarr_push_back_val(void* arr, void* src);
void* _dyarr_push_front_val(void* arr, void* src);
void* _dyarr_insert_val(void* arr, usize pos, void* src);

#define dyarr_push_val(_arr, _val) _arr = _dyarr_push_back_val(_arr, _val)
#define dyarr_push_front_val(_arr, _val)                                       \
    _arr = _dyarr_push_front_val(_arr, _val)
#define dyarr_insert_val(_arr, _pos, _val)                                     \
    _arr = _dyarr_insert_val(_arr, _pos, _val)

#define dyarr_push(_arr, _val)                                                 \
    ({                                                                         \
        typeof(_val) _tmp = _val;                                              \
        _arr              = _dyarr_push_back_val(_arr, &_tmp);                 \
    })

#define dyarr_push_front(_arr, _val)                                           \
    ({                                                                         \
        typeof(_val) _tmp = _val;                                              \
        _arr              = _dyarr_push_front_val(_arr, &_tmp);                \
    })

#define dyarr_insert(_arr, _pos, _val)                                         \
    ({                                                                         \
        typeof(_val) _tmp = _val;                                              \
        _arr              = _dyarr_insert_val(_arr, _pos, &_tmp);              \
    })

void dyarr_pop(void* arr, void* poppped);
void dyarr_pop_at(void* arr, usize pos, void* popped);

#endif
