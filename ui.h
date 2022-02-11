#ifndef UI_H
#define UI_H

#include <polynom.h>

void console_print_polynom(const struct polynom *p, void(*print)(const void*));
size_t console_read_sizet(const char* prompt, size_t min, size_t max);
struct polynom *console_read_polynom(
        struct coefType *type,
        void(*parseType)(void *dest, const char *str, char **end)
);




#endif // UI_H
