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



static int tests_main() {
    Suite *s = test_suite_polynom();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    return 0;
}

static int normal_main(void) {
//    struct coefType type = {
//        .size = sizeof(int),
//        .add = int_add,
//        .multiply = int_mult,
//        .zero = calloc(1, sizeof(int)),
//    };
//    struct polynom *a = console_read_polynom(&type, int_parse);
//    struct polynom *b = console_read_polynom(&type, int_parse);

//    polynom_add(a, b);
//    printf("Polynom: ");
//    console_print_polynom(a, int_print);

//    int arg = 1;
//    void* result = polynom_calc(a, &arg);
//    printf("Calculated value: ");
//    int_print(result);
//    printf("\n");
//    free(result);

//    polynom_delete(a);
//    polynom_delete(b);
//    free(type.zero);
//    console_menu(2, "f", "s");
//    return 0;
}

int main(int arg_count, char* arg_vals[]) {
    if(arg_count > 1 && strcmp(arg_vals[1], "test") == 0) {
        return tests_main();
    } else {
        return normal_main();
    }
}
