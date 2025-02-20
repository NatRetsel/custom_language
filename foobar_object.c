#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "foobar_object.h"

foobar_object_t* new_integer(vm_t* vm, int f){
	/*
		Creates a new integer object on the heap and track it within the vm.
		If memory allocation fails, NULL is returned. Calls the helper function _new_foobar_obj 
		to allocate memory and create the object.
		Parameters:
			vm: vm_t*
			f: int

		Returns:
			foobar_object_t*

		Example:
			foobar_object_t* foobar_obj = new_integer(vm, 1);
			printf("%d", foobar_obj->data.f_int);
			Output:
				1
				
	*/
	foobar_object_t* foobar_obj = _new_foobar_obj(vm);
	if (foobar_obj == NULL) return NULL;

	foobar_obj->kind = INTEGER;
	foobar_obj->data.f_int = f;
	
	return foobar_obj;
}

foobar_object_t* new_float(vm_t* vm, float f){
	/*
		Creates a new float object on the heap and track it within the vm.
		If memory allocation fails, NULL is returned.
		Parameters:
			vm: vm_t*
			f: float

		Returns:
			foobar_object_t*

		Example:
			foobar_object_t* foobar_obj = new_float(vm, 1.0);
			printf("%f", foobar_obj->data.f_float);
			Output:
				1.000000
	*/
	foobar_object_t* foobar_obj = _new_foobar_obj(vm);
	if (foobar_obj == NULL) return NULL;

	foobar_obj->kind = FLOAT;
	foobar_obj->data.f_float = f;
	
	return foobar_obj;
}

foobar_object_t* new_string(vm_t* vm, char* f){
	/*
		Creates a new string object on the heap and track it within the vm.
		If memory allocation fails, NULL is returned.
		Parameters:
			vm: vm_t*
			f: char*

		Returns:
			foobar_object_t*

		Example:
			foobar_object_t* foobar_obj = new_string(vm, "Hello World");
			printf("%s", foobar_obj->data.f_string);
			Output:
				Hello World
	*/
	foobar_object_t* foobar_obj = _new_foobar_obj(vm);
	if (foobar_obj == NULL) return NULL;
	char* foobar_str = malloc(strlen(f)+1);
	
	if (foobar_str == NULL){
		free(foobar_obj);
		return NULL;
	}

	strcpy(foobar_str, f);
	
	foobar_obj->kind = STRING;
	foobar_obj->data.f_string = foobar_str;
	return foobar_obj;
}

foobar_object_t* new_list(vm_t* vm, size_t size){
	/*
		Creates a new list object on the heap and track it within the vm.
		If memory allocation fails, NULL is returned.
		Parameters:
			vm: vm_t*
			size: size_t

		Returns:
			foobar_object_t*

		Example:
			foobar_object_t* foobar_obj = new_list(vm, 1);
			printf("%d", foobar_obj->data.f_list.size);
			Output:
				1
	*/
	foobar_object_t* foobar_obj = _new_foobar_obj(vm);
	if (foobar_obj == NULL) return NULL;
	foobar_object_t** list_elements = calloc(size, sizeof(foobar_object_t*));
	if (list_elements == NULL){
		free(foobar_obj);
		return NULL;
	}
	foobar_obj->kind = LIST;
	foobar_obj->data.f_list = (foobar_list_t){.size = size, .elements=list_elements};
	return foobar_obj;
}

foobar_object_t* list_get(foobar_object_t* foobar_obj, int index){
	/*
		Gets the element at the given index in the list.
		If the list is NULL or the index is out of bounds, NULL is returned.
		Parameters:
			foobar_obj: foobar_object_t*
			index: int
		Returns:
			foobar_object_t*

		Example:
			foobar_object_t* foobar_obj = new_list(vm, 1);
			foobar_object_t* foobar_obj_one = new_integer(vm, 1);
			foobar_object_t* foobar_obj_two = new_integer(vm, 2);
			list_set(foobar_obj, 0, foobar_obj_one);
			foobar_object_t* foobar_obj_three = list_get(foobar_obj, 0);
			printf("%d", foobar_obj_three->data.f_int);
			Output:
				1

	*/
	if (foobar_obj == NULL || foobar_obj->kind != LIST) return NULL;
	if (index < 0 || index > foobar_obj->data.f_list.size) return NULL;
	return foobar_obj->data.f_list.elements[index];
}

bool list_set(foobar_object_t* foobar_obj, int index, foobar_object_t* value){
	/*
		Sets the element at the given index in the list to the given value.
		If the list or value is NULL or the index is out of bounds, false is returned.
		Parameters:
			foobar_obj: foobar_object_t*
			index: int
			value: foobar_object_t*
		Returns:
			bool
		Example:
			foobar_object_t* foobar_obj = new_list(vm, 1);
			foobar_object_t* foobar_obj_one = new_integer(vm, 1);
			foobar_object_t* foobar_obj_two = new_integer(vm, 2);
			list_set(foobar_obj, 0, foobar_obj_one);
			foobar_object_t* foobar_obj_three = list_get(foobar_obj, 0);
			printf("%d", foobar_obj_three->data.f_int);
			Output:
				1
	*/
	if (foobar_obj == NULL || value == NULL || foobar_obj->kind != LIST) return false;
	if (index < 0 || index >= foobar_obj->data.f_list.size) return false;
	foobar_obj->data.f_list.elements[index] = value;
	return true;
}

foobar_object_t* foobar_add(vm_t* vm, foobar_object_t* foobar_obj_one, foobar_object_t* foobar_obj_two){
	/*
		Adds two foobar objects together.
		If the objects are of incompatible types, NULL is returned.
		Parameters:
			vm: vm_t*
			foobar_obj_one: foobar_object_t*
			foobar_obj_two: foobar_object_t*
		Returns:
			foobar_object_t*
				INTEGER + INTEGER = INTEGER
				INTEGER + FLOAT = FLOAT
				FLOAT + FLOAT = FLOAT
				STRING + STRING = STRING (CONCATENATE AND RETURN NEW STRING)
				LIST + LIST = LIST (EXTEND AND RETURN NEW STRING)

		Example:
			foobar_object_t* foobar_obj_one = new_integer(vm, 1);
			foobar_object_t* foobar_obj_two = new_integer(vm, 2);
			foobar_object_t* foobar_obj_three = foobar_add(vm, foobar_obj_one, foobar_obj_two);
			printf("%d", foobar_obj_three->data.f_int);
			Output:
				3

	*/
	switch (foobar_obj_one->kind){
		case INTEGER:
			if (foobar_obj_two->kind == INTEGER){
				return new_integer(vm, foobar_obj_one->data.f_int + foobar_obj_two->data.f_int);
			}
			if (foobar_obj_two->kind == FLOAT){
				return new_float(vm, foobar_obj_one->data.f_int + foobar_obj_two->data.f_float);
			}
			return NULL;

		case FLOAT:
			if (foobar_obj_two->kind == INTEGER){
				return new_float(vm, foobar_obj_one->data.f_float + foobar_obj_two->data.f_int);
			}
            if (foobar_obj_two->kind == FLOAT){
				return new_float(vm, foobar_obj_one->data.f_float + foobar_obj_two->data.f_float);
			}
			
			return NULL;

		case STRING:
			if (foobar_obj_two->kind == STRING){
				int length = strlen(foobar_obj_one->data.f_string) + strlen(foobar_obj_two->data.f_string) + 1;
				char *temp = calloc(length, sizeof(char));
	
				strcat(temp, foobar_obj_one->data.f_string);
				strcat(temp, foobar_obj_two->data.f_string);
				
				foobar_object_t * foobar_obj = new_string(vm, temp);
				free(temp);
				return foobar_obj;
			}
			return NULL;

		case LIST:
			if (foobar_obj_two->kind == LIST){
				int lengthone = foobar_obj_one->data.f_list.size;
				int lengthtwo = foobar_obj_two->data.f_list.size;
				foobar_object_t* foobar_list = new_list(vm, (size_t)(lengthone+lengthtwo));
				for (int i=0; i<lengthone; i++){
					list_set(foobar_list,i,foobar_obj_one->data.f_list.elements[i]);
				}

				for (int i=0; i<lengthtwo; i++){
					list_set(foobar_list,i+lengthone,foobar_obj_two->data.f_list.elements[i]);
				}
				return foobar_list;
			}
			return NULL;
	}
	return NULL;
}

foobar_object_t* _new_foobar_obj(vm_t* vm){
	/*
		Creates a new foobar object on the heap and track it within the vm.
		If memory allocation fails, NULL is returned.
		Parameters:
			vm: vm_t*
		Returns:
			foobar_object_t*
	*/
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;
    foobar_obj->is_marked = false;
	vm_track_object(vm, foobar_obj);
	return foobar_obj;
}

void foobar_object_free(foobar_object_t* obj){
	/*
		Frees the memory allocated for the foobar object.
		Frees the memory for stored objects within collection types first.
		Parameters:
			obj: foobar_object_t*

		Example:
			foobar_object_t* foobar_obj = new_integer(vm, 1);
			foobar_object_free(foobar_obj);

	*/
	switch (obj->kind){
		case INTEGER:
		case FLOAT:
			break;
		case STRING:
			free(obj->data.f_string);
			break;
		case LIST:{
			foobar_list_t* list = &obj->data.f_list;
			free(list->elements);
			list->elements = NULL;
			break;
		}
			
	}
	free(obj);
}