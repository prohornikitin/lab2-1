#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdnoreturn.h>
#include <stdarg.h>
#include <polynom.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>
#include <ui.h>


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
    int val = strtol(str, end, 10);
    memcpy(dest, &val, sizeof(int));
}


int main(void) {
    struct coefType type = {
        .size = sizeof(int),
        .add = int_add,
        .multiply = int_mult,
        .zero = calloc(1, sizeof(int)),
    };
    struct polynom *a = console_read_polynom(&type, int_parse);
    struct polynom *b = console_read_polynom(&type, int_parse);

    polynom_add(a, b);
    printf("Polynom: ");
    console_print_polynom(a, int_print);

    int arg = 1;
    void* result = polynom_calc(a, &arg);
    printf("Calculated value: ");
    int_print(result);
    printf("\n");
    free(result);

    polynom_delete(a);
    polynom_delete(b);
    free(type.zero);
    return 0;
}
