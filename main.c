#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>


void fatal_error(const char* str) {
    fprintf(stderr, "%s", str);
    exit(1);
}

struct Polynom {
    void* koefs;
    size_t koef_size;
    size_t degree;
};

struct Operations {
    void (*makeZero)(void*);
    void (*add)(void*, const void*);
    void (*multiply)(void*, const void*);
};


void add(struct Polynom *left, const struct Polynom right, const struct Operations ops) {
    if(left == NULL) {
        fatal_error("left addend is NULL");
        return;
    }
    if(left->degree != right.degree) {
        fatal_error("koef counts are not equal");
        return;
    }
    if(left->koef_size != right.koef_size) {
        fatal_error("koef type differs");
        return;
    }

    size_t count = left->degree;
    size_t size = left->koef_size;
    for(size_t i = 0; i < count; ++i) {
        ops.add(left->koefs + i * size, right.koefs + i * size);
    }
};

void multiplyByPolynom(struct Polynom *left, const struct Polynom right, const struct Operations ops) {
    if(left == NULL) {
        fatal_error("left multiplier is NULL");
        return;
    }
    if(left->koef_size != right.koef_size) {
        fatal_error("koef type differs");
        return;
    }

    size_t size = left->koef_size;
    size_t new_degree = left->degree + right.degree;
    void* new_koefs = malloc((new_degree+1) * size);
    void* buff = malloc(size);
    for(size_t i = 0; i <= new_degree; ++i) {
        ops.makeZero(new_koefs + (i * size));
    }
    for(size_t i = 0; i <= left->degree; ++i) {
        for(size_t j = 0; j <= right.degree; ++j) {
            void *mul1 = left->koefs + i * size;
            void *mul2 = right.koefs + j * size;
            memcpy(buff, mul1, size);
            ops.multiply(buff, mul2);
            void *result = new_koefs + ((i+j) * size);
            ops.add(result, buff);
        }
    }
    free(buff);
    free(left->koefs);
    left->koefs = new_koefs;
    left->degree = new_degree;
}

void multiplyBySkalar(struct Polynom *left, const void *right, const struct Operations ops) {
    if(left == NULL) {
        fatal_error("left multiplier is NULL");
        return;
    }
    size_t size = left->koef_size;
    for(size_t i = 0; i <= left->degree; ++i) {
        ops.multiply(left->koefs + i * size, right);
    }
}

void compose(struct Polynom *left, const struct Polynom right, const struct Operations ops) {
    for(size_t i = 0; i <= left->degree; ++i) {
        size_t size = left->koef_size;
        struct Polynom buff = {
            .koef_size = size,
            .degree = right.degree,
        };
        buff.koefs = malloc((buff.degree+1) * size);
        memcpy(&buff.koefs, right.koefs, buff.degree * size);
        for(size_t j = 1; j < i; j++) {
            multiplyByPolynom(&buff, right, ops);
        }
        multiplyBySkalar(&buff, left->koefs + i * size, ops);
        free(left->koefs);
        memcpy(left, &buff, sizeof(struct Polynom));
    }
}

void* calc(struct Polynom *polynom, const void* arg, const struct Operations ops) {
    size_t size = polynom->koef_size;
    void* result = malloc(size);
    void* addend = malloc(size);
    ops.makeZero(result);
    for(size_t i = 0; i <= polynom->degree; ++i) {
        memcpy(addend, polynom->koefs + size * i, size);
        for(size_t j = 1; j <= i; ++j) {
            ops.multiply(addend, arg);
        }
        ops.add(result, addend);
    }
    free(addend);
    return result;
}


void int_makeZero(void* av) {
    int *a = (int*)av;
    *a = 0;
}
void int_add(void* av, const void* bv) {
    int *a = (int*)av;
    int *b = (int*)bv;
    *a += *b;
}
void int_mult(void* av, const void* bv) {
    int *a = (int*)av;
    int *b = (int*)bv;
    *a *= *b;
}

int main(void) {
    int *koefs = malloc(3 * sizeof(int));
    koefs[0] = 1;
    koefs[1] = -2;
    koefs[2] = 1;
    struct Polynom a = {
        .koefs = koefs,
        .koef_size = sizeof(int),
        .degree = 2,
    };
    struct Operations ops = {
        .add = int_add,
        .multiply = int_mult,
        .makeZero = int_makeZero,
    };
    int *koefs2 = malloc(3 * sizeof(int));
    koefs[0] = 1;
    koefs[1] = 2;
    koefs[2] = 1;
    struct Polynom b = {
        .koefs = koefs2,
        .koef_size = sizeof(int),
        .degree = 2,
    };
    memcpy(b.koefs, a.koefs, 3 * sizeof(int));
    multiplyByPolynom(&a, b, ops);
    int arg = 2;
    int* result = calc(&b, &arg, ops);
    printf("%d\n", *result);
    result = calc(&a, &arg, ops);
    printf("%d\n", *result);
    free(koefs);
    free(koefs2);
    return 0;
}
