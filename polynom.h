#ifndef POLYNOM_H
#define POLYNOM_H


#include <stddef.h>

struct Polynom;
struct CoefType {
    size_t size;
    void (*makeZero)(void*);
    void (*add)(void*, const void*);
    void (*multiply)(void*, const void*);
    void (*print)(const void*);
};

struct Polynom *Polynom_new(struct CoefType *coefsType, size_t degree, void* koefs);
void Polynom_delete(struct Polynom *polynom);
void Polynom_print(const struct Polynom *p);
void Polynom_add(struct Polynom *left, const struct Polynom *right);
void Polynom_multByPolynom(struct Polynom *left, const struct Polynom *right);
void Polynom_multBySkalar(struct Polynom *left, const void *right);
void Polynom_compose(struct Polynom *left, const struct Polynom *right);
void* Polynom_calc(struct Polynom *p, const void* arg);

#endif // POLYNOM_H
