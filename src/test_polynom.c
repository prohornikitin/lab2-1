#include <check.h>
#include <polynom.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
#include <num_types.h>

static struct coefType *intType = NULL;


static void setup() {
    struct num_type *type = num_type_int();
    intType = coefType_new(
                type->size,
                type->zero,
                type->add,
                type->multiply
    );
    free(type);
}

static void teardown() {
    coefType_delete(intType);
}

void assertIntPolynom(struct polynom *p, ...) {
    size_t degree = polynom_get_degree(p);
    va_list args;
    va_start(args, p);
    for(size_t i = 0; i <= degree; ++i) {
        int expected = va_arg(args, int);
        const int real = *((const int*)polynom_get_coef(p, i));
        ck_assert_int_eq(real, expected);
    }
    va_end(args);


    ck_assert_int_eq(*((const int*)polynom_get_coef(p, degree+1)), 0);
}

struct polynom* newIntPolynom(size_t degree, ...) {
    int* coefs = malloc((degree+1) * sizeof(int));
    va_list args;
    va_start(args, degree);
    for(size_t i = 0; i <= degree; ++i) {
        coefs[i] = va_arg(args, int);
    }
    va_end(args);
    struct polynom *p = polynom_new(intType, degree, coefs);
    free(coefs);
    return p;
}

START_TEST(test_new) {
    struct polynom *a = newIntPolynom(1,    1, 2);
    assertIntPolynom(a, 1, 2);
    polynom_delete(a);
} END_TEST

START_TEST(test_add) {
    struct polynom *a = newIntPolynom(1,    1, 2);
    struct polynom *b = newIntPolynom(2,    0, -2, 3);
    polynom_add(a, b);
    assertIntPolynom(a, 1, 0, 3);
    polynom_delete(a);
    polynom_delete(b);
} END_TEST

START_TEST(test_mult_by_0) {
    struct polynom *a = newIntPolynom(1,    -1, 2);
    int k = 0;
    polynom_mult_by_skalar(a, &k);
    assertIntPolynom(a, 0, 0);
    polynom_delete(a);
} END_TEST

START_TEST(test_mult_by_1) {
    struct polynom *a = newIntPolynom(1,    -1, 2);
    int k = 1;
    polynom_mult_by_skalar(a, &k);
    assertIntPolynom(a, -1, 2);
    polynom_delete(a);
} END_TEST

START_TEST(test_mult_by_minus2) {
    struct polynom *a = newIntPolynom(1,    -1, 2);
    int k = -2;
    polynom_mult_by_skalar(a, &k);
    assertIntPolynom(a, 2, -4);
    polynom_delete(a);
} END_TEST

START_TEST(test_mult_by_polynom) {
    struct polynom *a = newIntPolynom(1,    -1,  2);
    struct polynom *b = newIntPolynom(2,     1, -2, 1);
    polynom_mult_by_polynom(a, b);
    assertIntPolynom(a, -1, 4, -5, 2);
    polynom_delete(a);
    polynom_delete(b);
} END_TEST

START_TEST(test_compose) {
    struct polynom *a = newIntPolynom(2,     1, 2, 1);
    struct polynom *b = newIntPolynom(1,    -1, 2);
    polynom_compose(a, b);
    assertIntPolynom(a, 0, 0, 4);
    polynom_delete(a);
    polynom_delete(b);
} END_TEST

START_TEST(test_calc_0) {
    struct polynom *a = newIntPolynom(2,     1, -2, 1);
    int arg = 0;
    int *result = polynom_calc(a, &arg);
    ck_assert_int_eq(*result, 1);
    free(result);
    polynom_delete(a);
} END_TEST

START_TEST(test_calc_minus1) {
    struct polynom *a = newIntPolynom(2,     1, 2, 1);
    int arg = -1;
    int* result = polynom_calc(a, &arg);
    ck_assert_int_eq(*result, 0);
    free(result);
    polynom_delete(a);
} END_TEST


Suite* test_suite_polynom(void) {
    Suite *s = suite_create("polynom");

    TCase *tc_new = tcase_create("new");
    tcase_add_unchecked_fixture(tc_new, setup, teardown);
    tcase_add_test(tc_new, test_new);
    suite_add_tcase(s, tc_new);

    TCase *tc_add = tcase_create("add");
    tcase_add_unchecked_fixture(tc_add, setup, teardown);
    tcase_add_test(tc_add, test_add);
    suite_add_tcase(s, tc_add);

    TCase *tc_mult_by_skalar = tcase_create("mult by skalar");
    tcase_add_unchecked_fixture(tc_mult_by_skalar, setup, teardown);
    tcase_add_test(tc_mult_by_skalar, test_mult_by_0);
    tcase_add_test(tc_mult_by_skalar, test_mult_by_1);
    tcase_add_test(tc_mult_by_skalar, test_mult_by_minus2);
    suite_add_tcase(s, tc_mult_by_skalar);

    TCase *tc_mult_by_polynom = tcase_create("mult by polynom");
    tcase_add_unchecked_fixture(tc_mult_by_polynom, setup, teardown);
    tcase_add_test(tc_mult_by_polynom, test_mult_by_polynom);
    suite_add_tcase(s, tc_mult_by_polynom);

    TCase *tc_compose = tcase_create("compose");
    tcase_add_unchecked_fixture(tc_compose, setup, teardown);
    tcase_add_test(tc_compose, test_compose);
    suite_add_tcase(s, tc_compose);

    TCase *tc_calc = tcase_create("compose");
    tcase_add_unchecked_fixture(tc_calc, setup, teardown);
    tcase_add_test(tc_calc, test_calc_0);
    tcase_add_test(tc_calc, test_calc_minus1);
    suite_add_tcase(s, tc_calc);

    return s;
}
