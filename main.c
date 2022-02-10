#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdnoreturn.h>
#include <stdarg.h>

noreturn void fatal_error(const char* str) {
    fprintf(stderr, "%s", str);
    exit(1);
}

struct Polynom {
    char* koefs;
    size_t koef_size;
    size_t degree;
};

struct Operations {
    void (*makeZero)(void*);
    void (*add)(void*, const void*);
    void (*multiply)(void*, const void*);
};

struct Polynom Polynom_new(size_t koef_size, size_t degree, void* koefs) {
    struct Polynom new = {
        .degree = degree,
        .koef_size = koef_size,
        .koefs = koefs
    };
    return new;
}

void intPolynom_print(const struct Polynom p) {
    int *const koefs = (void *const)p.koefs;
    for(size_t i = 0; i <= p.degree; ++i) {
        printf("%d\t", koefs[i]);
    }
    printf("\n");
}


void Polynom_add(struct Polynom *left, const struct Polynom right, const struct Operations ops) {
    if(left == NULL) {
        fatal_error("left addend is NULL");
    }
    if(left->koef_size != right.koef_size) {
        fatal_error("koef type differs");
    }

    if(right.degree > left->degree) {
        left->degree = right.degree;
        left->koefs = realloc(left->koefs, right.degree * right.koef_size);
    }

    size_t count = left->degree;
    size_t size = left->koef_size;
    for(size_t i = 0; i < count; ++i) {
        ops.add(left->koefs + i * size, right.koefs + i * size);
    }
}

void Polynom_multByPolynom(struct Polynom *left, const struct Polynom right, const struct Operations ops) {
    if(left == NULL) {
        fatal_error("left multiplier is NULL");
    }
    if(left->koef_size != right.koef_size) {
        fatal_error("koef type differs");
    }

    size_t size = left->koef_size;
    size_t new_degree = left->degree + right.degree;
    char* new_koefs = malloc((new_degree+1) * size);
    for(size_t i = 0; i <= new_degree; ++i) {
        ops.makeZero(new_koefs + (i * size));
    }
    void* buff = malloc(size);
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

void Polynom_multBySkalar(struct Polynom *left, const void *right, const struct Operations ops) {
    if(left == NULL) {
        fatal_error("left multiplier is NULL");
    }
    size_t size = left->koef_size;
    for(size_t i = 0; i <= left->degree; ++i) {
        ops.multiply(left->koefs + i * size, right);
    }
}

void Polynom_compose(struct Polynom *left, const struct Polynom right, const struct Operations ops) {
    size_t size = left->koef_size;
    size_t max_result_degree = right.degree * left->degree;
    struct Polynom result = {
                .koefs = malloc(size * (max_result_degree + 1)),
                .koef_size = size,
                .degree = max_result_degree
    };
    memcpy(result.koefs, left->koefs, size);
    for(size_t i = 1; i <= max_result_degree; ++i) {
        ops.makeZero(result.koefs + i * size);
    }
    for(size_t i = 1; i <= left->degree; ++i) {
        struct Polynom buff = {
            .koef_size = size,
            .degree = right.degree,
        };
        buff.koefs = malloc((buff.degree+1) * size);
        memcpy(buff.koefs, right.koefs, (buff.degree+1) * size);
        for(size_t j = 1; j < i; j++) {
            Polynom_multByPolynom(&buff, right, ops);
        }
        Polynom_multBySkalar(&buff, left->koefs + i * size, ops);
        Polynom_add(&result, buff, ops);
    }

    free(left->koefs);
    memcpy(left, &result, sizeof(struct Polynom));
}

void* Polynom_calc(struct Polynom *polynom, const void* arg, const struct Operations ops) {
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
    const int *b = (const int*)bv;
    *a += *b;
}
void int_mult(void* av, const void* bv) {
    int *a = (int*)av;
    const int *b = (const int*)bv;
    *a *= *b;
}

int main(void) {
    struct Operations ops = {
        .add = int_add,
        .multiply = int_mult,
        .makeZero = int_makeZero,
    };

    int *koefs = malloc(3 * sizeof(int));
    struct Polynom a = {
        .koefs = (char*)koefs,
        .koef_size = sizeof(int),
        .degree = 2,
    };
    koefs[0] = 1;
    koefs[1] = -2;
    koefs[2] = 1;

    int *koefs2 = malloc(3 * sizeof(int));
    struct Polynom b = {
        .koefs = (char*)koefs2,
        .koef_size = sizeof(int),
        .degree = 2,
    };
    koefs2[0] = 1;
    koefs2[1] = -2;
    koefs2[2] = 1;


    Polynom_compose(&a, b, ops);

    int arg = 0;
    int* result = Polynom_calc(&a, &arg, ops);
    printf("%d\n", *result);
    free(result);
    intPolynom_print(a);
    free(a.koefs);
    free(b.koefs);
    return 0;
}
