#ifndef NUM_TYPES_H
#define NUM_TYPES_H

#include <stddef.h>

struct num_type {
    size_t size;
    void *zero;
    void (*add)(void* left, const void* right);
    void (*multiply)(void* left, const void* right);
    void (*parse)(void *dest, const char *str, char **end);
    void (*print)(const void *value);
};

struct num_type *num_type_int(void);
struct num_type *num_type_double(void);
struct num_type *num_type_complex(void);
void del_num_type(struct num_type* );


#endif // NUM_TYPES_H
