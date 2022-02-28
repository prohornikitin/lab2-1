#include <polynom.h>

#include <stddef.h>
#include <errors_handle.h>
#include <string.h>
#include <stdlib.h>


struct polynom {
    char* coefs;
    struct coefType *coefsType;
    size_t degree;
};



struct coefType *coefType_new(
        size_t size,
        void *zero,
        void (*add)(void* left, const void* right),
        void (*multiply)(void* left, const void* right)
) {
    struct coefType* new = malloc(sizeof(struct coefType));
    new->add = add;
    new->multiply = multiply;
    new->size = size;
    new->zero = zero;
    return new;
}

void coefType_delete(struct coefType *t) {
    free(t->zero);
    free(t);
}

struct polynom *polynom_new(struct coefType *coefsType, size_t degree, void* coefs) {
    struct polynom* new = malloc(sizeof(struct polynom));
    size_t allCoefsSize = (degree+1) * coefsType->size;
    new->degree = degree;
    new->coefsType = coefsType;
    new->coefs = malloc(allCoefsSize);
    memcpy(new->coefs, coefs, allCoefsSize);
    return new;
}

void polynom_delete(struct polynom *polynom) {
    free(polynom->coefs);
    free(polynom);
}

struct polynom *polynom_copy(const struct polynom* p) {
    return polynom_new(p->coefsType, p->degree, p->coefs);
}

void polynom_add(struct polynom *left, const struct polynom *right) {
    if(memcmp(left->coefsType, right->coefsType, sizeof(struct coefType)) != 0) {
        FATAL_ERROR("koef type differs");
    }
    struct coefType *type = right->coefsType;
    size_t size = type->size;

    if(right->degree > left->degree) {
        left->coefs = realloc(left->coefs, (right->degree + 1) * size);
        for(size_t i = left->degree + 1; i <= right->degree; ++i) {
            memcpy(left->coefs + i * size, type->zero, size);

        }
        left->degree = right->degree;
    }

    for(size_t i = 0; i <= right->degree; ++i) {
        type->add(left->coefs + i * size, right->coefs + i * size);
    }
}

void polynom_mult_by_polynom(struct polynom *left, const struct polynom *right) {
    if(memcmp(left->coefsType, right->coefsType, sizeof(struct coefType)) != 0) {
        FATAL_ERROR("koef type differs");
    }
    struct coefType *type = right->coefsType;
    size_t size = type->size;

    size_t new_degree = left->degree + right->degree;
    char* new_coefs = malloc((new_degree+1) * size);
    for(size_t i = 0; i <= new_degree; ++i) {
        memcpy(new_coefs + (i * size), type->zero, size);
    }
    void* buff = malloc(type->size);
    for(size_t i = 0; i <= left->degree; ++i) {
        for(size_t j = 0; j <= right->degree; ++j) {
            void *mul1 = left->coefs + i * size;
            void *mul2 = right->coefs + j * size;
            memcpy(buff, mul1, size);
            type->multiply(buff, mul2);
            void *result = new_coefs + ((i+j) * size);
            type->add(result, buff);
        }
    }
    free(buff);
    free(left->coefs);
    left->coefs = new_coefs;
    left->degree = new_degree;
}

void polynom_mult_by_skalar(struct polynom *left, const void *right) {
    struct coefType *type = left->coefsType;
    for(size_t i = 0; i <= left->degree; ++i) {
        type->multiply(left->coefs + i * type->size, right);
    }
}

void polynom_compose(struct polynom *left, const struct polynom *right) {
    if(memcmp(left->coefsType, right->coefsType, sizeof(struct coefType)) != 0) {
        FATAL_ERROR("koef type differs");
    }
    struct coefType *type = right->coefsType;

    size_t max_result_degree = right->degree * left->degree;
    struct polynom result = {
                .coefs = malloc(type->size * (max_result_degree + 1)),
                .coefsType = type,
                .degree = max_result_degree,
    };
    memcpy(result.coefs, polynom_get_coef(left, 0), type->size);
    for(size_t i = 1; i <= max_result_degree; ++i) {
        memcpy(result.coefs + (i * type->size), type->zero, type->size);
    }
    for(size_t i = 1; i <= left->degree; ++i) {
        struct polynom *buff = polynom_copy(right);
        for(size_t j = 1; j < i; j++) {
            polynom_mult_by_polynom(buff, right);
        }
        polynom_mult_by_skalar(buff, polynom_get_coef(left, i));
        polynom_add(&result, buff);
        polynom_delete(buff);
    }
    free(left->coefs);
    memcpy(left, &result, sizeof(struct polynom));
}

void* polynom_calc(struct polynom *p, const void* arg) {
    struct coefType *type = p->coefsType;
    void* result = malloc(type->size);
    void* addend = malloc(type->size);
    memcpy(result, type->zero, type->size);
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

const void *polynom_get_coef(const struct polynom *p, size_t degree) {
    if(degree > p->degree) {
        return p->coefsType->zero;
    }
    return p->coefs + (degree * p->coefsType->size);
}

size_t polynom_get_degree(const struct polynom *p) {
    return p->degree;
}
