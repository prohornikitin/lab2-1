#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdnoreturn.h>
#include <stdarg.h>
#include <polynom.h>


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
void int_makeZero(void* av) {
    int *a = (int*)av;
    *a = 0;
}
void int_print(const void* vvalue) {
    const int* value = vvalue;
    if(*value >= 0) {
        printf("%d", *value);
    } else {
        printf("(%d)", *value);
    }

}
void int_fromStr(void *dest, const char *str, char **end) {
    int val = strtod(str, end);
    memcpy(dest, &val, sizeof(int));
}


int main(void) {
    struct CoefType type = {
        .size = sizeof(int),
        .add = int_add,
        .multiply = int_mult,
        .makeZero = int_makeZero,
        .print = int_print,
    };
    int *koefs = malloc(3 * sizeof(int));
    int *koefs2 = malloc(2 * sizeof(int));
    koefs[0] = 1;
    koefs[1] = -2;
    koefs[2] = 1;
    koefs2[0] = 1;
    koefs2[1] = -1;
    struct Polynom *a = Polynom_new(&type, 2, koefs);
    struct Polynom *b = Polynom_new(&type, 1, koefs2);

    Polynom_add(a, b);
    Polynom_print(a);

    int arg = 1;
    void* result = Polynom_calc(a, &arg);
    type.print(result);
    printf("\n");
    free(result);

    Polynom_delete(a);
    Polynom_delete(b);
    return 0;
}
