#ifndef UI_H
#define UI_H

#include <polynom.h>

void console_clear(void);
void wait_for_input(void);
void console_print_polynom(const struct polynom *p, void(*print)(const void*));
unsigned long long console_read_ull(const char* prompt, unsigned long long min, unsigned long max);
struct polynom *console_read_polynom(
        struct coefType *type,
        void(*parseType)(void *dest, const char *str, char **end)
);
int console_menu(size_t entries_count, /*entries*/...);

enum menu_action {
    ACT_INPUT = 1,
    ACT_COPY = 2,
    ACT_ADD = 3,
    ACT_MULT_BY_SKALAR = 4,
    ACT_MULT_BY_POLYNOM = 5,
    ACT_COMPOSE = 6,
    ACT_CALC = 7,
    ACT_OUTPUT = 8,
    ACT_EXIT = 9,
};
enum menu_action menu_action(void);

enum menu_input {
    INPUT_MANUAL = 1,
    INPUT_AUTO = 2,
};
enum menu_input menu_input(void);

enum menu_num_type {
    TYPE_INT = 1,
    TYPE_FLOAT = 2,
    TYPE_COMPLEX = 3,
};

enum menu_num_type menu_num_type(void);

#endif // UI_H
