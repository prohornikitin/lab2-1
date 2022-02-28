#include <num_types.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <complex.h>


static void int_add(void* av, const void* bv) {
    int *a = (int*)av;
    const int *b = (const int*)bv;
    *a += *b;
}
static void int_mult(void* av, const void* bv) {
    int *a = (int*)av;
    const int *b = (const int*)bv;
    *a *= *b;
}
static void int_print(const void* vvalue) {
    const int* value = vvalue;
    if(*value >= 0) {
        printf("%d", *value);
    } else {
        printf("(%d)", *value);
    }
}
static void int_parse(void *dest, const char *str, char **end) {
    long val = strtol(str, end, 10);
    if(val > INT_MAX || val < INT_MIN) {
        errno = ERANGE;
        return;
    }
    int intVal = (int)val;
    memcpy(dest, &intVal, sizeof(int));
}




struct num_type *num_type_int(void) {
    struct num_type *type = malloc(sizeof(struct num_type));
    type->size = sizeof(int);
    type->add = int_add;
    type->multiply = int_mult;
    type->zero = calloc(1, sizeof(int));
    type->parse = int_parse;
    type->print = int_print;
    return type;
}


static void double_add(void* av, const void* bv) {
    double *a = (double*)av;
    const double *b = (const double*)bv;
    *a += *b;
}
static void double_mult(void* av, const void* bv) {
    double *a = (double*)av;
    const double *b = (const double*)bv;
    *a *= *b;
}
static void double_print(const void* vvalue) {
    const double* value = vvalue;
    if(*value >= 0) {
        printf("%lf", *value);
    } else {
        printf("(%lf)", *value);
    }
}
static void double_parse(void *dest, const char *str, char **end) {
    double val = strtod(str, end);
    memcpy(dest, &val, sizeof(double));
}

struct num_type *num_type_double(void) {
    struct num_type *type = malloc(sizeof(struct num_type));
    type->size = sizeof(double);
    type->add = double_add;
    type->multiply = double_mult;
    type->zero = calloc(1, sizeof(double));
    type->parse = double_parse;
    type->print = double_print;
    return type;
}


static void complex_add(void* av, const void* bv) {
    double complex *a = (double complex*)av;
    const double complex *b = (const double complex*)bv;
    *a += *b;
}
static void complex_mult(void* av, const void* bv) {
    double complex *a = (double complex*)av;
    const double complex *b = (const double complex*)bv;
    *a *= *b;
}
static void complex_print(const void* vvalue) {
    const double complex* value = vvalue;
    printf("(%lf, %lf)", creal(*value), cimag(*value));
}
static void complex_parse(void *dest, const char *str, char **end) {
    if(str[0] != '(') {
        return;
    }
    str += 1;
    double real = strtod(str, end);
    str = *end;
    if(str[0] != ',') {
        return;
    }
    str += 1;
    double imag = strtod(str, end);
    str = *end;
    if(str[0] != ')') {
        return;
    }
    str += 1;
    *end = str;
    double complex value = real + imag * (double complex)I;
    memcpy(dest, &value, sizeof(value));
}

struct num_type *num_type_complex(void) {
    struct num_type *type = malloc(sizeof(struct num_type));
    type->size = sizeof(double complex);
    type->add = complex_add;
    type->multiply = complex_mult;
    type->parse = complex_parse;
    type->print = complex_print;
    double complex *zero = malloc(sizeof(double complex));
    *zero = 0;
    type->zero = zero;
    return type;
}

void del_num_type(struct num_type* type) {
    free(type->zero);
    free(type);
}
