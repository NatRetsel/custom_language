#include<stdlib.h>
#include "foobar_object.h"
#include "stack.h"
#include "vm.h"
#include "munit/munit.h"
#include "foobar_object.h"

static MunitResult
test_integer_constant(const MunitParameter params[], void* user_data){
    munit_assert_int(INTEGER, ==, 0);
    return MUNIT_OK;
}

static MunitResult
test_integer_obj(const MunitParameter params[], void* user_data){
    foobar_object_t* obj = malloc(sizeof(foobar_object_t));
    obj->kind = INTEGER;
    obj->data.f_int = 0;
    munit_assert_int(obj->kind, ==, INTEGER);
    munit_assert_int(obj->data.f_int, ==, 0);
    free(obj);

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {
        (char*) "test integer constant",
        test_integer_constant,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test integer object",
        test_integer_obj,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    "",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(){
    return munit_suite_main(&suite, (void*) "µnit", 0, NULL);
}
