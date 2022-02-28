#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdnoreturn.h>
#include <stdarg.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>
#include <polynom.h>
#include <ui.h>
#include <check.h>
#include <test_polynom.h>
#include <num_types.h>
#include <limits.h>
#include <vars.h>


static int tests_main() {
    Suite *s = test_suite_polynom();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void* get_existing_variable(struct vars* vars, const char* prompt) {
    void* var;
    do {
        char* name = readline(prompt);
        var = var_get(vars, name);
        free(name);
        if(var == NULL) {
            printf("Variable does not exists. Try again\n");
        }
    } while(var == NULL);
    return var;
}




static struct num_type *get_num_type() {
    enum menu_num_type type = menu_num_type();
    switch(type) {
        case TYPE_INT: return num_type_int();
        case TYPE_FLOAT: return num_type_double();
        case TYPE_COMPLEX: return num_type_complex();
    }
}

struct polynom *action_input(struct num_type *type, struct vars *vars) {
    console_clear();
    char* var_name = readline("Variable name: ");
    struct coefType *coefType = coefType_new(
        type->size,
        type->zero,
        type->add,
        type->multiply
    );
    struct polynom *polynom = console_read_polynom(coefType, type->parse);
    var_set(vars, var_name, polynom);
    free(var_name);
}

void action_add(struct num_type *type, struct vars* vars) {
    console_clear();
    struct polynom *r = get_existing_variable(vars, "Left operand var (where the result will be stored): ");
    struct polynom *l = get_existing_variable(vars, "Right operand var: ");
    polynom_add(l,r);
    printf("Successfully Added. Result = ");
    console_print_polynom(l, type->print);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

void action_calc(struct num_type *type, struct vars* vars) {
    struct polynom* polynom = get_existing_variable(vars,"Polynome var name: ");

    void* arg = malloc(type->size);
    char* arg_str = readline("Argument: ");
    char *end;
    type->parse(arg, arg_str, &end);
    free(arg_str);

    void* result = polynom_calc(polynom, arg);
    free(arg);
    printf("Successfully Calculated. Result = ");
    type->print(result);
    free(result);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

void action_output(struct num_type *type, struct vars* vars) {
    console_clear();
    struct polynom *p = get_existing_variable(vars, "Polynom var name: ");
    console_print_polynom(p, type->print);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

void action_copy(struct vars* vars) {
    console_clear();
    struct polynom *src = get_existing_variable(vars, "Source var name: ");
    char *new_var_name = readline("Destination var name: ");
    var_set(vars, new_var_name, polynom_copy(src));
    printf("\nSuccesfully copied to \"%s\"\n", new_var_name);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

void action_compose(struct num_type* type, struct vars* vars) {
    console_clear();
    struct polynom *l = get_existing_variable(vars, "Left operand var (where the result will be stored): ");
    struct polynom *r = get_existing_variable(vars, "Right operand var: ");
    polynom_compose(l, r);
    printf("\nSuccesfully composed. Result = ");
    console_print_polynom(l, type->print);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

void action_mult_by_polynom(struct num_type *type, struct vars *vars) {
    console_clear();
    struct polynom *l = get_existing_variable(vars, "Left operand var (where the result will be stored): ");
    struct polynom *r = get_existing_variable(vars, "Right operand var: ");
    polynom_mult_by_polynom(l, r);
    printf("\nSuccesfully multiplied. result = ");
    console_print_polynom(l, type->print);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

void action_mult_by_skalar(struct num_type *type, struct vars *vars) {
    console_clear();
    struct polynom *polynom = get_existing_variable(vars, "Polynom to multiply (var): ");

    void* skalar = malloc(type->size);
    char* arg_str = readline("Multiplier: ");
    char *end;
    type->parse(skalar, arg_str, &end);
    free(arg_str);

    polynom_mult_by_skalar(polynom, skalar);
    printf("\nSuccesfully multiplied. result = ");
    console_print_polynom(polynom, type->print);
    printf("\nPress any key to continue\n");
    wait_for_input();
}

static int normal_main(void) {
    struct num_type* type = get_num_type();
    struct vars *vars = vars_new();


    bool needToExit = false;
    while(!needToExit) {
        switch(menu_action()) {
        case ACT_INPUT:
            action_input(type, vars);
            break;
        case ACT_ADD:
            action_add(type, vars);
            break;
        case ACT_CALC:
            action_calc(type, vars);
            break;
        case ACT_OUTPUT:
            action_output(type, vars);
            break;
        case ACT_COPY:
            action_copy(vars);
            break;
        case ACT_COMPOSE:
            action_compose(type, vars);
            break;
        case ACT_MULT_BY_POLYNOM:
            action_mult_by_polynom(type, vars);
            break;
        case ACT_MULT_BY_SKALAR:
            action_mult_by_skalar(type, vars);
            break;
        case ACT_EXIT:
            needToExit = true;
            break;
        }
    }


    vars_delete(vars);
    return 0;
}

int main(int arg_count, char* arg_vals[]) {
    if(arg_count > 1 && strcmp(arg_vals[1], "test") == 0) {
        return tests_main();
    } else {
        return normal_main();
    }
}
