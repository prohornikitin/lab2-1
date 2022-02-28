#include <ui.h>
#include <stdio.h>


enum menu_input menu_input(void) {
    console_clear();
    printf("Choose input type:\n");
    return (enum menu_input) console_menu(2, "Manual", "Auto");
}

enum menu_action menu_action(void) {
    console_clear();
    printf("Binary polynome operations save result in first polynome!\n");
    printf("Choose an action:\n");
    return (enum menu_action) console_menu(9,
                                          "Input",
                                          "Copy",
                                          "Add",
                                          "Multiply by scalar",
                                          "Multiply by polynome",
                                          "Compose",
                                          "Calculate with given arg",
                                          "Output",
                                          "Exit");
}

enum menu_num_type menu_num_type(void) {
    console_clear();
    printf("Choose numbers type:\n");
    return (enum menu_num_type) console_menu(3,
                                          "Integer",
                                          "Floating Point",
                                          "Complex");
}

