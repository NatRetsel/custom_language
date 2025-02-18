#include<stdlib.h>
#include<stdio.h> 
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
test_float_constant(const MunitParameter params[], void* user_data){
    munit_assert_int(FLOAT, ==, 1);
    return MUNIT_OK;
}

static MunitResult
test_string_constant(const MunitParameter params[], void* user_data){
    munit_assert_int(STRING, ==, 2);
    return MUNIT_OK;
}

static MunitResult
test_list_constant(const MunitParameter params[], void* user_data){
    munit_assert_int(LIST, ==, 3);
    return MUNIT_OK;
}

static MunitResult
test_integer_obj(const MunitParameter params[], void* user_data){
    foobar_object_t* obj = malloc(sizeof(foobar_object_t));
    obj->kind = INTEGER;
    obj->data.f_int = 0;
    munit_assert_int(obj->kind, ==, INTEGER);
    munit_assert_int(obj->data.f_int, ==, 0);
    foobar_object_free(obj);
    obj = NULL;
    munit_assert_ptr(obj,==,NULL);

    return MUNIT_OK;
}

static MunitResult
test_float_obj(const MunitParameter params[], void* user_data){
    foobar_object_t* obj = malloc(sizeof(foobar_object_t));
    obj->kind = FLOAT;
    obj->data.f_float = 0.0;
    munit_assert_int(obj->kind, ==, FLOAT);
    munit_assert_float(obj->data.f_float, ==, 0.0);
    free(obj);
    obj = NULL;
    munit_assert_ptr_null(obj);
    return MUNIT_OK;
}

static MunitResult
test_string_obj(const MunitParameter params[], void* user_data){
    foobar_object_t* obj = malloc(sizeof(foobar_object_t));
    char* foobar_str = malloc(strlen("Hello World")+1);
    strcpy(foobar_str, "Hello World");
    obj->kind = STRING;
    obj->data.f_string = foobar_str;
    munit_assert_int(obj->kind, ==, STRING);
    munit_assert_string_equal(obj->data.f_string, "Hello World");
    foobar_object_free(obj);
    obj = NULL;
    munit_assert_ptr_null(obj);
    return MUNIT_OK;
}

static MunitResult
test_list_obj(const MunitParameter params[], void* user_data){
    foobar_object_t* obj = malloc(sizeof(foobar_object_t));
    obj->kind = LIST;
    obj->data.f_list.size = 0;
    obj->data.f_list.elements = malloc(sizeof(foobar_object_t*));
    munit_assert_int(obj->kind, ==, LIST);
    munit_assert_int(obj->data.f_list.size, ==, 0);
    munit_assert_ptr_not_null(obj->data.f_list.elements);
    foobar_object_free(obj);
    obj = NULL;
    munit_assert_ptr_null(obj);
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
        (char*) "test float constant",
        test_float_constant,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test string constant",
        test_string_constant,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test list constant",
        test_list_constant,
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
    {
        (char*) "test float object",
        test_float_obj,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test string object",
        test_string_obj,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test list object",
        test_list_obj,
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
