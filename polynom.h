#ifndef POLYNOM_H
#define POLYNOM_H


#include <stddef.h>

struct polynom;
struct coefType {
    size_t size;
    void *zero;
    void (*add)(void* left, const void* right);
    void (*multiply)(void* left, const void* right);
};

struct polynom *polynom_new(struct coefType *coefsType, size_t degree, void* coefs);
void polynom_delete(struct polynom *polynom);
const void* polynom_get_coef(const struct polynom *p, size_t degree);
size_t polynom_get_degree(const struct polynom *p);
void polynom_add(struct polynom *left, const struct polynom *right);
void polynom_mult_by_polynom(struct polynom *left, const struct polynom *right);
void polynom_mult_by_skalar(struct polynom *left, const void *right);
void polynom_compose(struct polynom *left, const struct polynom *right);
void* polynom_calc(struct polynom *p, const void* arg);


#endif // POLYNOM_H
