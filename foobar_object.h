#pragma once
#include <stdbool.h>
#include "vm.h"

typedef struct FooBarObject foobar_object_t;

typedef enum FooBarObjectKind {
	// Add supported types here
	INTEGER,
	FLOAT,
	STRING,
	LIST
}foobar_object_kind_t;

typedef struct FooBarList{
	size_t size;
	foobar_object_t** elements;
}foobar_list_t;

typedef union FooBarObjectData{
	// Add data of supported types here
	int f_int;
	float f_float;
	char* f_string;
	foobar_list_t f_list;
}foobar_object_data_t;

typedef struct FooBarObject{
	// Our template struct
    bool is_marked;
	foobar_object_kind_t kind;
	foobar_object_data_t data;
}foobar_object_t;

foobar_object_t* new_integer(vm_t* vm, int f);
foobar_object_t* new_float(vm_t* vm, float f);
foobar_object_t* new_string(vm_t* vm, char* f);
foobar_object_t* new_list(vm_t* vm, size_t size);
foobar_object_t* list_get(foobar_object_t* foobar_obj, int index);
bool list_set(foobar_object_t* foobar_obj, int index, foobar_object_t* value);
foobar_object_t* foobar_add(vm_t* vm, foobar_object_t* foobar_obj_one, foobar_object_t* foobar_obj_two);
foobar_object_t* _new_foobar_obj(vm_t* vm);
void foobar_object_free(foobar_object_t* obj);