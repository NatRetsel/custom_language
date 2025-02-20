#include<stdlib.h>
#include<stdio.h> 
#include "foobar_object.h"
#include "stack.h"
#include "vm.h"
#include "munit/munit.h"
#include "foobar_object.h"

static MunitResult
test_integer_constant(const MunitParameter params[], void* user_data){
    // 1. Assert that INTEGER is an enum (0)
    munit_assert_int(INTEGER, ==, 0);
    return MUNIT_OK;
}

static MunitResult
test_float_constant(const MunitParameter params[], void* user_data){
    // 1. Assert that FLOAT is an enum (1)
    munit_assert_int(FLOAT, ==, 1);
    return MUNIT_OK;
}

static MunitResult
test_string_constant(const MunitParameter params[], void* user_data){
    // 1. Assert that STRING is an enum (2)
    munit_assert_int(STRING, ==, 2);
    return MUNIT_OK;
}

static MunitResult
test_list_constant(const MunitParameter params[], void* user_data){
    // 1. Assert that LIST is an enum (3)
    munit_assert_int(LIST, ==, 3);
    return MUNIT_OK;
}

static MunitResult
test_integer_obj(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for foobar_object_t
    2. Assign enum of type INTEGER
    3. Assign data
    4. Assert that the kind is INTEGER
    5. Assert that the data is 0
    6. Free memory
    */
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
    /*
    1. Allocate memory for foobar_object_t
    2. Assign enum of type FLOAT
    3. Assign data
    4. Assert that the kind is FLOAT
    5. Assert that the data is 0.0
    6. Free memory
    */
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
    /*
    1. Allocate memory for foobar_object_t
    2. Allocate memory for string to be included in string foobar object
    3. Assign enum of type STRING
    4. Assign data
    4. Assert that the kind is STRING
    5. Assert that the data is the string assigned
    6. Free memory
    */
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
    /*
    1. Allocate memory for foobar_object_t
    2. Allocate memory for foobar_object_t* to be included in list foobar object
    3. Assign enum of type LIST
    4. Assign data
    4. Assert that the kind is STRING
    5. Assert that the data (list element) pointer is not NULL
    6. Free memory
    */
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

static MunitResult
test_integer_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two integer foobar_object_t* (two integers)
    3. Call foobar add on the two integer foobar_object_t
    4. Assert that the returned foobar_objet_t* kind is INTEGER
    5. Assert that the returned foobar_object_t* data is the sum of the two integers
    6. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_integer(vm, 2);
    foobar_object_t* obj2 = new_integer(vm, 1);
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_int(obj3->kind, ==, INTEGER);
    munit_assert_int(obj3->data.f_int, ==, 3);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_float_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two integer foobar_object_t* (two float)
    3. Call foobar add on the two float foobar_object_t
    4. Assert that the returned foobar_objet_t* kind is FLOAT
    5. Assert that the returned foobar_object_t* data is the sum of the two FLOAT
    6. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_float(vm, 2.0);
    foobar_object_t* obj2 = new_float(vm, 1.0);
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_int(obj3->kind, ==, FLOAT);
    munit_assert_double(obj3->data.f_float, ==, 3.0);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_integer_float_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two float foobar_object_t* (two float)
    3. Call foobar add on the two float foobar_object_t
    4. Assert that the returned foobar_objet_t* kind is FLOAT
    5. Assert that the returned foobar_object_t* data is the sum of the two FLOAT
    6. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_integer(vm, 2);
    foobar_object_t* obj2 = new_float(vm, 1.0);
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_int(obj3->kind, ==, FLOAT);
    munit_assert_double(obj3->data.f_float, ==, 3.0);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_float_integer_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two foobar_object_t* (one integer, one float)
    3. Call foobar add on the two foobar_object_t
    4. Assert that the returned foobar_objet_t* kind is FLOAT
    5. Assert that the returned foobar_object_t* data is the sum of the integer and float
    6. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_float(vm, 2.0);
    foobar_object_t* obj2 = new_integer(vm, 1);
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_int(obj3->kind, ==, FLOAT);
    munit_assert_double(obj3->data.f_float, ==, 3.0);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_string_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two foobar_object_t* (two strings of different string value)
    3. Call foobar add on the two foobar_object_t
    4. Assert that the returned foobar_objet_t* kind is STRING
    5. Assert that the returned foobar_object_t* data is the concatenation of the two string values
    6. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_string(vm, "hello");
    foobar_object_t* obj2 = new_string(vm, "world");
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_int(obj3->kind, ==, STRING);
    munit_assert_string_equal(obj3->data.f_string, "helloworld");
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_integer_string_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two foobar_object_t* (one string and one integer)
    3. Call foobar add on the two foobar_object_t
    4. Assert that the returned foobar_objet_t* is NULL
    5. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_integer(vm, 2);
    foobar_object_t* obj2 = new_string(vm, "world");
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_ptr_null(obj3);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_string_integer_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two foobar_object_t* (one string and one integer)
    3. Call foobar add on the two foobar_object_t
    4. Assert that the returned foobar_objet_t* is NULL
    5. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_string(vm, "hello");
    foobar_object_t* obj2 = new_integer(vm, 2);
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_ptr_null(obj3);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_float_string_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two foobar_object_t* (one string and one float)
    3. Call foobar add on the two foobar_object_t
    4. Assert that the returned foobar_objet_t* is NULL
    5. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_float(vm, 2.0);
    foobar_object_t* obj2 = new_string(vm, "world");
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_ptr_null(obj3);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_string_float_add(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate two foobar_object_t* (one string and one float)
    3. Call foobar add on the two foobar_object_t
    4. Assert that the returned foobar_objet_t* is NULL
    5. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_string(vm, "hello");
    foobar_object_t* obj2 = new_float(vm, 2.0);
    foobar_object_t* obj3 = foobar_add(vm, obj, obj2);
    munit_assert_ptr_null(obj3);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;

}

static MunitResult
test_list_set_get(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate three foobar_object_t* (one list and two integers)
    3. Call list set on the list with the two integers
    4. Assert that the set was successful after each call
    5. Retrieve the two integers from the list
    6. Assert that the retrieved integers are the same as the two integers (kind == INTEGER, data matches)
    7. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_list(vm, 2);
    foobar_object_t* obj2 = new_integer(vm, 2);
    foobar_object_t* obj3 = new_integer(vm, 3);
    bool set_res = list_set(obj, 0, obj2);
    munit_assert_true(set_res);
    set_res = list_set(obj, 1, obj3);
    munit_assert_true(set_res);

    foobar_object_t* obj2ref = list_get(obj, 0);
    munit_assert_int(obj2ref->kind, ==, INTEGER);
    munit_assert_int(obj2ref->data.f_int, ==, 2);

    foobar_object_t* obj3ref = list_get(obj, 1);
    munit_assert_int(obj3ref->kind, ==, INTEGER);
    munit_assert_int(obj3ref->data.f_int, ==, 3);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    obj2ref = NULL;
    obj3ref = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(obj2ref);
    munit_assert_ptr_null(obj3ref);
    munit_assert_ptr_null(vm);
    return MUNIT_OK;
}

static MunitResult
test_invalid_index_list_set_get(const MunitParameter params[], void* user_data){
    /*
    1. Allocate memory for vm
    2. Instantiate four foobar_object_t* (one list and three integers)
    3. Call list set on the list with the first two integers
    4. Assert that the set was successful after each call
    5. Call list set on the third integer
    6. Assert that the set was unsuccessful
    7. Call list get for the third index (2)
    8. Assert that the returned foobar_object_t* is NULL
    9. Call list get for invalid index (-1)
    10 Assert that the returned foobar_object_t* is NULL
    11. Free memory
    */
    vm_t* vm = vm_new();
    foobar_object_t* obj = new_list(vm, 2);
    foobar_object_t* obj2 = new_integer(vm, 2);
    foobar_object_t* obj3 = new_integer(vm, 3);
    foobar_object_t* obj4 = new_integer(vm, 4);
    bool set_res = list_set(obj, 0, obj2);
    munit_assert_true(set_res);
    set_res = list_set(obj, 1, obj3);
    munit_assert_true(set_res);
    set_res = list_set(obj, 2, obj4);
    munit_assert_false(set_res);
    foobar_object_t* obj4ref = list_get(obj, 2);
    munit_assert_ptr_null(obj4ref);
    foobar_object_t* invalid_obj = list_get(obj, -1);
    munit_assert_ptr_null(invalid_obj);
    vm_collect_garbage(vm);
    vm_free(vm);
    obj = NULL;
    obj2 = NULL;
    obj3 = NULL;
    obj4 = NULL;
    obj4ref = NULL;
    vm = NULL;
    munit_assert_ptr_null(obj);
    munit_assert_ptr_null(obj2);
    munit_assert_ptr_null(obj3);
    munit_assert_ptr_null(obj4);
    munit_assert_ptr_null(obj4ref);
    munit_assert_ptr_null(vm);
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
    {
        (char*) "test integer add",
        test_integer_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test float add",
        test_float_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test integer float add",
        test_integer_float_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test string add",
        test_string_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test integer string add",
        test_integer_string_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test string integer add",
        test_string_integer_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test float string add",
        test_float_string_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test string float add",
        test_string_float_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test list set get",
        test_list_set_get,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char*) "test invalid index list set get",
        test_invalid_index_list_set_get,
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
