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
    int val = strtol(str, end, 10);
    memcpy(dest, &val, sizeof(int));
}


long console_readLong(const char* prompt, long min, long max) {
    bool isError = true;
    long num = 0;
    do {
        char* s = readline(prompt);
        if (!s) {
            printf("EOF consumed, number expected. Try again.\n");
            continue;
        }

        char* end;
        num = strtol(s, &end, 10);
        if (end == s || *end != '\0') {
            printf("Expected a number. Try again.\n");
        } else if (errno == ERANGE || (num < min) || (num > max)) {
            printf("Entered number is not in a valid range. It must be from %ld to %ld.\n", min, max);
        } else {
            isError = false;
        }
        free(s);
    } while (isError);

    return num;
}

struct Polynom *console_readPolynom(struct CoefType *type) {
    size_t degree = console_readLong("Polynom degree: ", 0, __LONG_MAX__);

    char* coefs = NULL;
    bool isError = false;
    do {
        isError = false;
        char* str = readline("Polynom coefficients separated by spaces: ");

        coefs = malloc(type->size * (degree + 1));
        char* str_i = str;
        for(size_t i = 0; i <= degree; i++) {
            char *end = str_i;
            type->fromStr(coefs + (i*type->size), str_i, &end);
            if(end == str_i) {
                isError = true;
                break;
            }
            if(*end == ' ' && i != degree) {
                str_i = end+1;
            } else if(*end != '\0' || i != degree) {
                isError = true;
                break;
            }
        }
        free(str);
        if(isError) {
            printf("Сan't parse the coefs. Please, try again.\n");
            free(coefs);
        }
    } while (isError);
    return Polynom_new(type, degree, coefs);
}

int main(void) {
    struct CoefType type = {
        .size = sizeof(int),
        .add = int_add,
        .multiply = int_mult,
        .makeZero = int_makeZero,
        .print = int_print,
        .fromStr = int_fromStr,
    };
    struct Polynom *a = console_readPolynom(&type);
    struct Polynom *b = console_readPolynom(&type);

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
