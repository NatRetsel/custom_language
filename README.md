# custom_language
Intentions of this exercise:
* Appreciate abstraction of lower level management in high level languages like Python.
* Solidify low level concepts.
* Credits to team at boot.dev ((link to lesson)[https://www.youtube.com/watch?v=rJrd2QMVbGM&pp=ygUfbWVtb3J5IG1hbmFnZW1lbnQgaW4gYyBib290IGRldg%3D%3D])


Let's call our language - FooBarLang (I asked ChatGPT for suggestions)

### Supported variable types
We will first create various variable and collection types:
* Integer
* Float
* String
* List 
* Stack (for garbage collector simulation)

Later we'll write our garbage collector (a code simulated one) since we are wrapping these types in structs and we have no way of knowing required size in compile time. While each of the types mentioned above are structurally different, we'll wrap everything under a template struct.

#### Template Struct
Let's call our template struct `foobar_object_t`. This template contains the information to the variable type (Integer, Float, etc) as well as its data. For ease of variable type iteration and processing, we will house the variable types in an enum `foobar_object_kind_t`. We will store the data in a union `foobar_object_data_t`. Creation of `foobar_object_t` will be abstracted via functions.

A sample of the template struct, enum and union below:
`foobar_object.h`
```c
typedef enum FooBarObjectKind {
	// Add supported types here
	INTEGER,
	FLOAT
}foobar_object_kind_t;

typedef union FooBarObjectData{
	// Add data of supported types here
	int f_int;
	float f_float;
}foobar_object_data_t;

typedef struct FooBarObject{
	// Our template struct
	foobar_object_kind_t kind;
	foobar_object_data_t data;
}foobar_object_t;
```

In each of our abstraction functions, we will attempt to allocate memory on the heap for a `foobar_objec_t`, which will also handle `foobar_object_kind_t` and `foobar_object_data_t`. A cool note on `foobar_object_data_t`, memory is allocated for the largest type specified in the union.

#### Integer
Our `FooBarLang` integer is straightforward and holds the primitive `int` type in C and `INTEGER` as its kind. Our function `new_integer` function takes in the `int` data value and returns `foobar_object_t`

```c
foobar_object_t* new_integer(int f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type INTEGER
	foobar_obj->kind = INTEGER;

	// Assign data
	foobar_obj->data.f_int = f;
	
	return foobar_obj;
}
```

#### Float
In similar fashion, we have a `new_float` function catered for `float` types.
```c
foobar_object_t* new_float(float f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type FLOAT
	foobar_obj->kind = FLOAT;

	// Assign data
	foobar_obj->data.f_float = f;
	
	return foobar_obj;
}
```

#### String
We'll now include support for the `string` type, which is `char*` under the hood.

`foobar_object.h`
```c
typedef enum FooBarObjectKind {
	// Add supported types here
	INTEGER,
	FLOAT,
	STRING
}foobar_object_kind_t;

typedef union FooBarObjectData{
	// Add data of supported types here
	int f_int;
	float f_float;
	char* f_string;
}foobar_object_data_t;

typedef struct FooBarObject{
	// Our template struct
	foobar_object_kind_t kind;
	foobar_object_data_t data;
}foobar_object_t;

foobar_object_t* new_integer(int f);
foobar_object_t* new_float(float f);
foobar_object_t* new_string(char* f);
```

`foobar_object.c`
```c
foobar_object_t* new_integer(int f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type INTEGER
	foobar_obj->kind = INTEGER;

	// Assign data
	foobar_obj->data.f_int = f;
	
	return foobar_obj;
}

foobar_object_t* new_float(float f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type FLOAT
	foobar_obj->kind = FLOAT;

	// Assign data
	foobar_obj->data.f_loat = f;
	
	return foobar_obj;
}

foobar_object_t* new_string(char* f){
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
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
```

Since strings are `char*` and decay to pointers, we have to make a copy of the provided string argument.

### List
Unlike native arrays in `C`, `FooBarLang`'s `list` is able to dynamically resize each time its capacity is maxed out. We'll use a simple doubling to resize each time the original array is full.

A new struct `foobar_list_t` is created as a `foobar_object_data_t` representing our `FooBarLang` `list`. `foobar_list_t` contains information about its current size and a pointer to pointer of `foobar_object_t`. Pointer to pointer was used as the various `foobar_object_t` are created at runtime - allocated on the heap and accessed through `malloc`'s return pointer.

`new_list` takes in a `size_t` parameter:
* attempts to allocate memory for `foobar_object_t` (our  `foobar_object` template) and `foobar_object_t**` (`list` elements)
* creates `foobar_list_t` and initializes the `size` and `**elements` accordingly.
* initializes the `kind` and `data` appropriately for `foobar_object_t`.

`foobar_object.h`
```c
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
	foobar_object_kind_t kind;
	foobar_object_data_t data;
}foobar_object_t;

foobar_object_t* new_integer(int f);
foobar_object_t* new_float(float f);
foobar_object_t* new_string(char* f);
foobar_object_t* new_list(size_t size);
```

`foobar_object.c`
```c
foobar_object_t* new_integer(int f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type INTEGER
	foobar_obj->kind = INTEGER;

	// Assign data
	foobar_obj->data.f_int = f;
	
	return foobar_obj;
}

foobar_object_t* new_float(float f){
	// Allocate memory on the heap for foobar_object_t
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;

	// Assign enum of type FLOAT
	foobar_obj->kind = FLOAT;

	// Assign data
	foobar_obj->data.f_float = f;
	
	return foobar_obj;
}

foobar_object_t* new_string(char* f){
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
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

foobar_object_t* new_list(size_t size){
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
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
```

#### Get and Set via indexing
We'll be using helper functions for `list` indexing - both getting and setting. `list_get` takes in a pointer to `foobar_object_t` and an `int` `index`. If `foobar_object_t` is not a `LIST` or `NULL` we return `NULL`. We also return `NULL` when the supplied `index` is invalid (less than 0 or greater than `data.f_list.size`).

`foobar_object.h`
```c
foobar_object_t* list_get(foobar_object_t* foobar_obj, int index);
```

`foobar_object.c`
```c
foobar_object_t* list_get(foobar_object_t* foobar_obj, int index){
	if (foobar_obj == NULL || foobar_obj->kind != LIST) return NULL;
	if (index < 0 || index > foobar_obj->data.f_list.size) return NULL;
	return foobar_obj->data.f_list.elements[index];
}
```

In similar essence with our function for setting, `list_set` takes in a. pointer to `foobar_object_t`, an `int` `index` but this time with a `foobar_object_t*` `value` element to be set into the index. We return differing response depending if setting is successful (`true`) or not (`false`). Setting is failed if the provided `foobar_obj` is `NULL` or not a `LIST` or if the `index` is invalid or if `value` is `NULL`.


`foobar_object.h`
```c
bool list_set(foobar_object_t* foobar_obj, int index, foobar_object_t* value);
```

`foobar_object.c`
```c
bool list_set(foobar_object_t* foobar_obj, int index, foobar_object_t* value){
	if (foobar_obj == NULL || value == NULL || foobar_obj->kind != LIST) return false;
	if (index < 0 || index > foobar_obj->data.f_list.size) return false;
	foobar_obj->data.f_list.elements[index] = value;
	return true;
}
```


#### Supported operations
Now let's brainstorm on the allowed operations for our supported types.

| S/N | Operation      | Types                          | Expected Outcome                                        |
| --- | -------------- | ------------------------------ | ------------------------------------------------------- |
| 1   | Add            | int<br>float<br>string<br>list | int addition<br>float addition<br>concatenate<br>extend |
| 2   | Subtract       | int<br>float                   | int subtraction<br>float subtraction                    |
| 3   | Multiplication | int<br>float                   | int multiplication<br>float multiplication              |
| 4   | Division       | int<br>float                   | int division<br>float division                          |

Each of these operations will be done through an equivalent function. A check will be done in each function to distill the `kind`, which determines the steps and return variable. As an example, below is the implementation of `foobar_add`. It takes in 2 pointers of `foobar_object_t` as argument. Similar implementation can be used for the other operations.

`foobar_object.h`
```c
foobar_object_t* foobar_add(foobar_object_t* foobar_obj_one, foobar_object_t* foobar_obj_two);
```

`foobar_object.c`
```c
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
				int lengthtwo = foobar_obj_two->data.f_list_size;
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
```

### Garbage collection
As we intend `FooBarLang` to be a high level language, we are abstracting the lower level memory management away. This entails freeing up allocated memory that are not in use. There are various algorithms determining if variables are still required (e.g. Reference counting and Mark-and-Sweep). We will focus on explaining reference counting and implement Mark-and-Sweep as the better alternative to reference counting.

#### Reference counting
In reference counting, we track the number of references made to each object; every object has a reference counter - it is incremented with each new reference, decrement with each reference that has been garbage collected. When any object's reference count reaches zero, the object is garbage collected. 

#### Mark-and-Sweep - Preparation
Although simple, the reference counting algorithm is not able to handle the case of cycles; when we have two objects and within each there are reference to the other object.
```c
foobar_object_t* first = new_list(1);
foobar_object_t* second = new_list(2);
foobar_object_t* third = new_list(3);
// reference count of first: 1, reference count of second: 1

list_set(first, 0, second);
// reference count of first: 1, reference count of second: 2
list_set(second, 0, third);
// refere count: first:1, second: 2, third: 2
// In the event first reference count drops to 0, there are no parts of the code that reference with second and third, and reference counting along does not reduces their count to 0
ref_dec(first);
// ref count of first: 0, ref count of second: 1, third: 2
```

The tradeoff with MaS is the higher memory overhead and possibility of poor performance when there are plenty of objects to be freed.

![[Mark_and_sweep.png]]

Roots are non-heap references - theses are references the program code use to reference objects on the heap and are the ones directly accessible from user code. Iterating these roots, we process the child objects accessible following the reference chain (pointer chasing). Each time we follow the reference chain, the objects are marked. At the end of the pointer chasing process, we iterate across all objects. Objects that are unmarked are deemed safe to remove. 

For the purposes of illustrations, we will be constructing a code simulated virtual machine. We will define a `vm_t` type that contains 2 stacks - one for stack frames (`frames`) and one for `foobar_object_t` objects (`objects`). With every new scope (e.g function call), a new stack frame is pushed into `frames` and each time we exit a scope, the top `frame` is popped.

![[vm_gc_exercise.png]]

`stack.h`
```c
#include <stddef.h>
#include <stdlib.h>

typedef struct Stack {
  size_t count; //no. of elements
  size_t capacity; //size
  void **data; // no specific data format
} stack_t;

stack_t *stack_new(size_t capacity);

void stack_push(stack_t *stack, void *obj);
void *stack_pop(stack_t *stack);

void stack_free(stack_t *stack);
void stack_remove_nulls(stack_t *stack);
```


`stack.c`
```c
#include <stdio.h>

#include "stack.h"

void stack_push(stack_t *stack, void *obj) {
  if (stack->count == stack->capacity) {
    // Double stack capacity to avoid reallocing often
    stack->capacity *= 2;
    stack->data = realloc(stack->data, stack->capacity * sizeof(void *));
    if (stack->data == NULL) {
      // Unable to realloc, just exit :) get gud
      exit(1);
    }
  }

  stack->data[stack->count] = obj;
  stack->count++;

  return;
}

void *stack_pop(stack_t *stack) {
  if (stack->count == 0) {
    return NULL;
  }

  stack->count--;
  return stack->data[stack->count];
}

void stack_free(stack_t *stack) {
  if (stack == NULL) {
    return;
  }

  if (stack->data != NULL) {
    free(stack->data);
  }

  free(stack);
}

void stack_remove_nulls(stack_t *stack) {
  size_t new_count = 0;

  // Iterate through the stack and compact non-NULL pointers.
  for (size_t i = 0; i < stack->count; ++i) {
    if (stack->data[i] != NULL) {
      stack->data[new_count++] = stack->data[i];
    }
  }

  // Update the count to reflect the new number of elements.
  stack->count = new_count;

  // Optionally, you might want to zero out the remaining slots.
  for (size_t i = new_count; i < stack->capacity; ++i) {
    stack->data[i] = NULL;
  }
}

stack_t *stack_new(size_t capacity) {
  stack_t *stack = malloc(sizeof(stack_t));
  if (stack == NULL) {
    return NULL;
  }

  stack->count = 0;
  stack->capacity = capacity;
  stack->data = malloc(stack->capacity * sizeof(void *));
  if (stack->data == NULL) {
    free(stack);
    return NULL;
  }

  return stack;
}
```

`vm.h`
```c
#include "stack.h"

typedef struct VirtualMachine {
  stack_t *frames;
  stack_t *objects;
} vm_t;

vm_t *vm_new();
void vm_free(vm_t *vm);
```

We will assume our `vm` has stack size of `8` for both `frames` and `objects`.

`vm.c`
```c
#include "vm.h"
vm_t *vm_new(){
	vm_t* vm = malloc(sizeof(vm_t));
	if (vm == NULL) return NULL;
	vm->frames = stack_new(8);
	vm->objects = stack_new(8);
	return vm;
}

void vm_free(vm_t *vm){
	if (vm != NULL){
		stack_free(vm->frames);
		stack_free(vm->objects);
		free(vm);
	}
	return;
}
```

We'll also be creating a `frame_t` struct - simplifying the notion that they are stack frames in `vm`.

`vm.h`
```c
typedef struct StackFrame {
  stack_t *references;
} frame_t;

void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm);

void frame_free(frame_t *frame);
```

`vm.c`
```c
void vm_frame_push(vm_t *vm, frame_t *frame) {
	// note we could use stack_push as well, but stick with
	// frame_push to be pedantic about pushing frame_t into vm_t
  if (vm == NULL || frame == NULL) return;
  stack_push(vm->frames, frame);
  return;
}

frame_t *vm_new_frame(vm_t *vm) {
	// instantiate new frame_t obj and the inner reference stack 
	// before pushing into frame in vm
  frame_t* frame_obj = malloc(sizeof(frame_t));
  if (frame_obj == NULL) return NULL;
  frame_obj->references = stack_new(8);
  vm_frame_push(vm->frames, frame_obj);
  return frame_obj;
}

void frame_free(frame_t *frame) {
	// free the reference stack within frame_t, then free frame
  if (frame == NULL || frame->references == NULL) return;
  stack_free(frame->references);
  free(frame);
  return;
}
```

We'll have a function that pushes `foobar_object_t` into the `vm object` stack as part of tracking (`vm_track_object`). We'll also engage the help of a new function `_new_foobar_object` that tries to allocate memory of size `foobar_object_t` and calls `vm_track_object`.

`vm.c`
```c
#include "foobar_object.h"
void vm_track_object(vm_t *vm, foobar_object_t *obj) {
  if (vm == NULL || obj == NULL) return;
  switch (obj->kind){
    case INTEGER:
    case FLOAT:
    case STRING:
    case LIST:
      stack_push(vm->objects, obj);
      break;
  }
  return;
}
```

`foobar_object.h`
```c
foobar_object_t* _new_foobar_obj(vm_t* vm);
```

`foobar_object.c`
```c
foobar_object_t* _new_foobar_obj(vm_t* vm){
	foobar_object_t* foobar_obj = malloc(sizeof(foobar_object_t));
	if (foobar_obj == NULL) return NULL;
	vm_track_object(vm->objects, foobar_obj);
	return foobar_obj;
}
```

We'll modify the creation function of the other `foobar_object_t` types, replacing memory allocation lines with the function call to `_new_foobar_obj`.

Let's have a function to free the `foobar_object_t` objects. The function first determines the kind of `foobar_object_t` is then applies the appropriate freeing procedure. For objects containing nested objects within, we will free those first.

`foobar_object.h`
```c
void foobar_object_free(foobar_object_t* obj);
```

`foobar_object.c`
```c
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
```

Let's also write a function to free our `vm`. We free the frames in the frame stack and `foobar_object_t` tracked in the object stack. Then we free the respective stacks before the `vm` object.

`vm.h`
```c
void vm_free(vm_t* vm);
```

`vm.c`
```c
void vm_free(vm_t* vm){
	if (vm == NULL) return;
	for (int i=0; i<vm->frames->count; i++){
		frame_free(vm->frames->data[i]);
	}
	stack_free(vm->frames);
	for (int i=0; i<vm->objects->count; i++){
		foobar_object_free(vm->objects->data[i]);
	}
	stack_free(vm->objects);
	free(vm);
}
```

Let's also add a `frame_reference_object` function pushing `foobar_object_t` objects into `frame_t` frame.

`vm.h`
```c
void frame_reference_object(frame_t* frame, foobar_object_t* obj);
```

`vm.c`
```c
void frame_reference_object(frame_t *frame, foobar_object_t *obj) {
  if (frame == NULL || obj == NULL) return;
  stack_push(frame->references, obj);
  return;
}
```

#### Mark
Our `foobar_object_t` objects will require a new field to distinguish marked objects from unmarked ones. Newly initialized objects from `_new_foobar_object` are first labelled as unmarked. We will use a boolean variable `is_marked` for this.

`foobar_object.h`
```c
typedef struct FooBarObject {
  bool is_marked;
  snek_object_kind_t kind;
  snek_object_data_t data;
} foobar_object_t;
```

`foobar_object.c`
```c
foobar_object_t *_new_foobar_object(vm_t *vm) {
  foobar_object_t *foobar_obj = calloc(1, sizeof(foobar_object_t));
  if (foobar_obj == NULL) {
    return NULL;
  }
  foobar_obj->is_marked = false;
  vm_track_object(vm, foobar_obj);
  return foobar_obj;
}
```

To mark objects, we will iterate over each frame in the VM then iterate over each reference in each frame and change `is_marked = true`

`vm.h`
```c
void mark(vm_t* vm);
```

`vm.c`
```c
void mark(vm_t* vm){
	for (int i=0; i<vm->frames->count; i++){
		frame_t* frame = vm->frames->data[i];
		for (int j=0; j<frame->references->count; j++){
			foobar_object_t* foobar_obj = frame->references->data[i];
			foobar_obj->is_marked = true;
		}
	}
}
```
#### Trace
Tracing is part of the mark phase. In the trace phase, we first create a stack object meant to contain objects that are marked. Then for each of these marked objects, we need to check if they contain references to other objects and if they are marked. `trace` is the function where the initial stack creation and pushing of marked objects into the stack happens. As we go through items in the marked stack, we call `trace_blacken_objects` to trace objects nested in collections. This calls a helper function `trace_mark_object` to push marked nested-objects into the stack.

`vm.h`
```c
void trace(vm_t *vm);
void trace_blacken_object(stack_t *gray_objects, foobar_object_t *obj);
void trace_mark_object(stack_t *gray_objects, foobar_object_t *obj);
```

`vm.c`
```c
void trace_blacken_object(stack_t* gray_objects, foobar_object_t* obj);
void trace(vm_t *vm){
	stack_t* gray_obj = stack_new(8);
	if (gray_obj == NULL) return;
	for (int i=0; i<vm->objects->count; i++){
		foobar_object_t* obj = vm->objects->data[i];
		if (obj->is_marked == true){
			stack_push(gray_obj, obj);
		}
	}
	while (gray_obj->count > 0){
		foobar_object_t* obj = stack_pop(gray_obj);
		trace_blacken_object(gray_obj, obj);
	}
	stack_free(gray_obj);
}

void trace_blacken_objects(stack_t* gray_objects, foobar_object_t* obj){
	if (gray_objects == NULL || obj == NULL) return;
	switch (obj->kind){
		case INTEGER:
		case FLOAT:
		case STRING:
			break;
		case LIST:
			for (int i=0; i<obj->data.v_array.size; i++){
	        trace_mark_object(gray_objects, obj->data.v_array.elements[i]);
		    }
		    break;
	}
	return;
}

void trace_mark_object(stack_t *gray_objects, foobar_object_t *obj) {
  if (obj == NULL || obj->is_marked == true) return;
  obj->is_marked = true;
  stack_push(gray_objects, obj);
  return;
}
```

#### Sweep
This function iterates all of VM's objects (`vm->objects`). If object is marked, unmark it (so that they can be removed in the next call if it is unused) and continue else this object is deemed unused and remove it. As we remove, we set the data in that position to `NULL`. Afterwards we'll call our `stack_remove_nulls` helper function to tidy the VM's tack.

`vm.h`
```c
void sweep(vm_t* vm);
```

`vm.c`
```c
void sweep(vm_t *vm) {
  for (int i=0; i<vm->objects->count; i++){
    foobar_object_t *foobar_obj = vm->objects->data[i];
    if (foobar_obj->is_marked == true) {
      foobar_obj->is_marked = false;
      continue;
    }else{
      foobar_object_free(snek_obj);
      vm->objects->data[i] = NULL;
    }
  }
  stack_remove_nulls(vm->objects);
  return;
}
```

#### Collect garbage
We now have our essential parts for garbage collection. This high level function `vm_collect_garbage` simply calls `mark`, `trace` and `sweep` in succession.

`vm.h`
```c
void sweep(vm_t* vm);
void vm_collect_garbage(vm_t *vm);
```

`vm.c`
```c
void vm_collect_garbage(vm_t *vm) {
  mark(vm);
  trace(vm);
  sweep(vm);
  return;
}
```
