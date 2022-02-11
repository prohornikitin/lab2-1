#include <polynom.h>

#include <stddef.h>
#include <fatal_error.h>
#include <string.h>
#include <stdlib.h>


struct Polynom {
    char* coefs;
    struct CoefType *coefsType;
    size_t degree;
};



//struct CoefType CoefType_new(
//        size_t size,
//        void (*makeZero)(void*),
//        void (*add)(void*, const void*),
//        void (*multiply)(void*, const void*),
//        void (*print)(const void*)
//) {
//    struct CoefType type = {
//        .size = size,
//        .makeZero = makeZero,
//        .add = add,
//        .multiply = multiply,
//        .print = print,
//    };
//    return type;
//}

struct Polynom *Polynom_new(struct CoefType *coefsType, size_t degree, void* coefs) {
    struct Polynom* new = malloc(sizeof(struct Polynom));
    new->degree = degree;
    new->coefsType = coefsType;
    new->coefs = coefs;
    return new;
}

void Polynom_delete(struct Polynom *polynom) {
    free(polynom->coefs);
    free(polynom);
}

void Polynom_add(struct Polynom *left, const struct Polynom *right) {
    if(left == NULL) {
        FATAL_ERROR("left addend is NULL");
    }
    if(right == NULL) {
        FATAL_ERROR("right addend is NULL");
    }
    if(memcmp(left->coefsType, right->coefsType, sizeof(struct CoefType)) != 0) {
        FATAL_ERROR("koef type differs");
    }
    struct CoefType *type = right->coefsType;
    size_t size = type->size;

    if(right->degree > left->degree) {
        left->coefs = realloc(left->coefs, (right->degree + 1) * size);
        for(size_t i = left->degree + 1; i <= right->degree; ++i) {
            type->makeZero(left->coefs + i * size);
        }
        left->degree = right->degree;
    }

    for(size_t i = 0; i <= right->degree; ++i) {
        type->add(left->coefs + i * size, right->coefs + i * size);
    }
}

void Polynom_multByPolynom(struct Polynom *left, const struct Polynom *right) {
    if(left == NULL) {
        FATAL_ERROR("left multiplier is NULL");
    }
    if(left->coefsType != right->coefsType) {
        FATAL_ERROR("koef type differs");
    }
    struct CoefType *type = right->coefsType;

    size_t new_degree = left->degree + right->degree;
    char* new_koefs = malloc((new_degree+1) * type->size);
    for(size_t i = 0; i <= new_degree; ++i) {
        type->makeZero(new_koefs + (i * type->size));
    }
    void* buff = malloc(type->size);
    for(size_t i = 0; i <= left->degree; ++i) {
        for(size_t j = 0; j <= right->degree; ++j) {
            void *mul1 = left->coefs + i * type->size;
            void *mul2 = right->coefs + j * type->size;
            memcpy(buff, mul1, type->size);
            type->multiply(buff, mul2);
            void *result = new_koefs + ((i+j) * type->size);
            type->add(result, buff);
        }
    }
    free(buff);
    free(left->coefs);
    left->coefs = new_koefs;
    left->degree = new_degree;
}

void Polynom_multBySkalar(struct Polynom *left, const void *right) {
    if(left == NULL) {
        FATAL_ERROR("left multiplier is NULL");
    }
    struct CoefType *type = left->coefsType;
    for(size_t i = 0; i <= left->degree; ++i) {
        type->multiply(left->coefs + i * type->size, right);
    }
}

void Polynom_compose(struct Polynom *left, const struct Polynom *right) {
    if(left == NULL) {
        FATAL_ERROR("left multiplier is NULL");
    }
    struct CoefType *type = right->coefsType;

    size_t max_result_degree = right->degree * left->degree;
    struct Polynom result = {
                .coefs = malloc(type->size * (max_result_degree + 1)),
                .coefsType = type,
                .degree = max_result_degree,
    };
    memcpy(result.coefs, left->coefs, type->size);
    for(size_t i = 1; i <= max_result_degree; ++i) {
        type->makeZero(result.coefs + (i * type->size));
    }
    struct Polynom buff = {
        .coefsType = type,
        .degree = right->degree,
    };
    buff.coefs = malloc((buff.degree+1) * type->size);
    for(size_t i = 1; i <= left->degree; ++i) {
        memcpy(buff.coefs, right->coefs, (buff.degree+1) * type->size);
        for(size_t j = 1; j < i; j++) {
            Polynom_multByPolynom(&buff, right);
        }
        Polynom_multBySkalar(&buff, left->coefs + (i * type->size));
        Polynom_add(&result, &buff);
    }
    free(buff.coefs);
    free(left->coefs);
    memcpy(left, &result, sizeof(struct Polynom));
}

void* Polynom_calc(struct Polynom *p, const void* arg) {
    struct CoefType *type = p->coefsType;
    void* result = malloc(type->size);
    void* addend = malloc(type->size);
    type->makeZero(result);
    for(size_t i = 0; i <= p->degree; ++i) {
        memcpy(addend, p->coefs + (type->size * i), type->size);
        for(size_t j = 1; j <= i; ++j) {
            type->multiply(addend, arg);
        }
        type->add(result, addend);
    }
    free(addend);
    return result;
}

void Polynom_print(const struct Polynom *p) {
    const char* coefs = p->coefs;
    struct CoefType *type = p->coefsType;
    for(size_t i = p->degree; i > 0; --i) {
        type->print(coefs + (i * type->size));
        printf("*x^%zu + ", i);
    }
    type->print(coefs);
    printf("\n");
}

