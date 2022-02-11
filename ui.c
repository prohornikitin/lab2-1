#include <stddef.h>
#include <polynom.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <readline/readline.h>
#include <stdlib.h>


void console_print_polynom(const struct polynom *p, void(*print)(const void*) ) {
    for(size_t i = polynom_get_degree(p); i > 0; --i) {
        print(polynom_get_coef(p, i));
        printf("*x^%zu + ", i);
    }
    print(polynom_get_coef(p, 0));
    printf("\n");
}

size_t console_read_sizet(const char* prompt, size_t min, size_t max) {
    bool isError = true;
    size_t num = 0;
    do {
        char* s = readline(prompt);
        if (!s) {
            printf("EOF consumed, number expected. Try again.\n");
            continue;
        }

        char* end = NULL;
        num = strtoull(s, &end, 10);
        if (end == s || *end != '\0') {
            printf("Expected an integer number. Try again.\n");
        } else if (errno == ERANGE || (num < min) || (num > max) || s[0] == '-') {
            printf("Entered number is not in a valid range. It must be from %zu to %zu.\n", min, max);
        } else {
            isError = false;
        }
        free(s);
    } while (isError);

    return num;
}

struct polynom *console_read_polynom(
        struct coefType *type,
        void(*parseType)(void *dest, const char *str, char **end)
) {
    size_t degree = console_read_sizet("Polynom degree: ", 0, __LONG_MAX__);

    char* coefs = malloc(type->size * (degree + 1));
    bool isError = false;
    do {
        isError = false;
        char* str = readline("Polynom coefficients separated by spaces: ");
        char* str_i = str;
        for(size_t i = 0; i <= degree; i++) {
            char *end = str_i;
            parseType(coefs + (i*type->size), str_i, &end);
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
        }
    } while (isError);
    return polynom_new(type, degree, coefs);
}
