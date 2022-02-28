#include <stddef.h>
#include <polynom.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <stdint.h>
#if defined(__linux__)
    #include <termios.h>
    #include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #include <conio.h>
#endif


void console_clear(void) {
    #if defined(__linux__)
        system("clear");
    #elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        system("cls");
    #endif
}

void wait_for_input(void) {
    #if defined(__linux__)
        struct termios oldt,newt;
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~( (unsigned int)ICANON | (unsigned int)ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
        getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    #elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        getch();
    #endif
}


void console_print_polynom(const struct polynom *p, void(*print)(const void*)) {
    for(size_t i = polynom_get_degree(p); i > 0; --i) {
        print(polynom_get_coef(p, i));
        printf("*x^%zu + ", i);
    }
    print(polynom_get_coef(p, 0));
    printf("\n");
}

unsigned long long console_read_ull(const char* prompt, unsigned long long min, unsigned long long max) {
    bool isError = true;
    unsigned long long num = 0;
    do {
        char* s = readline(prompt);
        if (!s) {
            printf("EOF consumed, number expected. Try again.\n");
            continue;
        }

        char* end = NULL;
        if(sizeof(size_t) == sizeof(unsigned long long)) {
            num = strtoull(s, &end, 10);
        } else if(sizeof(size_t) == sizeof(unsigned long)) {
            num = strtoull(s, &end, 10);
        }
        if (end == s || *end != '\0') {
            printf("Expected an integer number. Try again.\n");
        } else if (errno == ERANGE || (num < min) || (num > max) || s[0] == '-') {
            printf("Entered number is not in a valid range. It must be from %llu to %llu.\n", min, max);
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
    size_t degree = console_read_ull("Polynom degree: ", 0, SIZE_MAX);

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
    struct polynom *p = polynom_new(type, degree, coefs);
    free(coefs);
    return p;
}

size_t console_menu(size_t entries_count, ...) {
    va_list entries;
    va_start(entries, entries_count);
    printf("\n");
    console_clear();
    for(size_t i = 0; i < entries_count; ++i) {
        printf("%zu) %s\n", i+1, va_arg(entries, const char*));
    }
    va_end(entries);
    return console_read_ull("Option number: ", 1, entries_count);
}
