#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "foobar_object.h"

foobar_object_t* new_integer(vm_t* vm, int f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = _new_foobar_obj(vm);
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type INTEGER
	foobar_obj->kind = INTEGER;

	// Assign data
	foobar_obj->data.f_int = f;
	
	return foobar_obj;
}

foobar_object_t* new_float(vm_t* vm, float f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = _new_foobar_obj(vm);
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type FLOAT
	foobar_obj->kind = FLOAT;

	// Assign data
	foobar_obj->data.f_float = f;
	
	return foobar_obj;
}

foobar_object_t* new_string(vm_t* vm, char* f){
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
	if (foobar_obj == NULL || foobar_obj->kind != LIST) return NULL;
	if (index < 0 || index > foobar_obj->data.f_list.size) return NULL;
	return foobar_obj->data.f_list.elements[index];
}

bool list_set(foobar_object_t* foobar_obj, int index, foobar_object_t* value){
	if (foobar_obj == NULL || value == NULL || foobar_obj->kind != LIST) return false;
	if (index < 0 || index > foobar_obj->data.f_list.size) return false;
	foobar_obj->data.f_list.elements[index] = value;
	return true;
}

foobar_object_t* foobar_add(foobar_object_t* foobar_obj_one, foobar_object_t* foobar_obj_two){

	switch (foobar_obj_one->kind){
		case INTEGER:
			if (foobar_obj_two->kind == INTEGER){
				return new_int(foobar_obj_one->data.f_int + foobar_obj_two->data.f_int);
			}
			if (foobar_obj_two->kind == FLOAT){
				return new_float(foobar_obj_one->data.f_int + foobar_obj_two->data.f_float);
			}
			return NULL;

		case FLOAT:
			if (foobar_obj_two->kind == INTEGER){
				return new_float(foobar_obj_one->data.f_float + foobar_obj_two->data.f_int);
			}
            if (foobar_obj_two->kind == FLOAT){
				return new_float(foobar_obj_one->data.f_float + foobar_obj_two->data.f_float);
			}
			
			return NULL;

		case STRING:
			if (foobar_obj_two->kind == STRING){
				int length = strlen(foobar_obj_one->data.f_string) + strlen(foobar_obj_two->data.f_string) + 1;
				char *temp = calloc(length, sizeof(char));
	
				strcat(temp, foobar_obj_one->data.f_string);
				strcat(temp, foobar_obj_two->data.f_string);
				
				foobar_object_t * foobar_obj = new_string(temp);
				free(temp);
				return foobar_obj;
			}
			return NULL;

		case LIST:
			if (foobar_obj_two->kind == LIST){
				int lengthone = foobar_obj_one->data.f_list.size;
				int lengthtwo = foobar_obj_two->data.f_list.size;
				foobar_object_t* foobar_list = new_list((size_t)(lengthone+lengthtwo));
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
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;
    foobar_obj->is_marked = false;
	vm_track_object(vm->objects, foobar_obj);
	return foobar_obj;
}

void foobar_object_free(foobar_object_t* obj){
	switch (obj->kind){
		case INTEGER:
		case FLOAT:
			break;
		case STRING:
			free(obj->data.f_string);
			break;
		case LIST:
			foobar_object_t* list = obj->data.f_list;
			free(list->elements);
			break;
	}
	free(obj);
}