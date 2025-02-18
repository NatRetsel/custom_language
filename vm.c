#include <stddef.h>
#include "vm.h"
#include "foobar_object.h"

void trace_blacken_objects(stack_toy_t* gray_objects, foobar_object_t* obj);

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
vm_t *vm_new(){
	vm_t* vm = malloc(sizeof(vm_t));
	if (vm == NULL) return NULL;
	vm->frames = stack_new(8);
	vm->objects = stack_new(8);
	return vm;
}

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
  vm_frame_push(vm, frame_obj);
  return frame_obj;
}

void frame_free(frame_t *frame) {
	// free the reference stack within frame_t, then free frame
  if (frame == NULL || frame->references == NULL) return;
  stack_free(frame->references);
  free(frame);
  frame = NULL;
  return;
}

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
	vm = NULL;
}

void frame_reference_object(frame_t *frame, foobar_object_t *obj) {
  if (frame == NULL || obj == NULL) return;
  stack_push(frame->references, obj);
  return;
}

void mark(vm_t* vm){
	for (int i=0; i<vm->frames->count; i++){
		frame_t* frame = vm->frames->data[i];
		for (int j=0; j<frame->references->count; j++){
			foobar_object_t* foobar_obj = frame->references->data[i];
			foobar_obj->is_marked = true;
		}
	}
}

void trace_mark_object(stack_toy_t *gray_objects, foobar_object_t *obj) {
  if (obj == NULL || obj->is_marked == true) return;
  obj->is_marked = true;
  stack_push(gray_objects, obj);
  return;
}

void trace_blacken_object(stack_toy_t* gray_objects, foobar_object_t* obj){
	if (gray_objects == NULL || obj == NULL) return;
	switch (obj->kind){
		case INTEGER:
		case FLOAT:
		case STRING:
			break;
		case LIST:
			for (int i=0; i<obj->data.f_list.size; i++){
	        	trace_mark_object(gray_objects, obj->data.f_list.elements[i]);
		    }
		    break;
	}
	return;
}

void trace(vm_t *vm){
	stack_toy_t* gray_obj = stack_new(8);
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





void sweep(vm_t *vm) {
  for (int i=0; i<vm->objects->count; i++){
    foobar_object_t *foobar_obj = vm->objects->data[i];
    if (foobar_obj->is_marked == true) {
      foobar_obj->is_marked = false;
      continue;
    }else{
      foobar_object_free(foobar_obj);
      vm->objects->data[i] = NULL;
    }
  }
  stack_remove_nulls(vm->objects);
  return;
}

void vm_collect_garbage(vm_t *vm) {
  mark(vm);
  trace(vm);
  sweep(vm);
  return;
}