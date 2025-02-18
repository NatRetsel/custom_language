#pragma once
#include "stack.h"
#include <stdlib.h>
typedef struct FooBarObject foobar_object_t;


typedef struct VirtualMachine {
  stack_toy_t *frames;
  stack_toy_t *objects;
} vm_t;

typedef struct StackFrame {
  stack_toy_t *references;
} frame_t;

void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm);

void frame_free(frame_t *frame);

vm_t *vm_new();
void vm_free(vm_t *vm);

void vm_track_object(vm_t *vm, foobar_object_t *obj);
void vm_free(vm_t* vm);
void frame_reference_object(frame_t* frame, foobar_object_t* obj);
void mark(vm_t* vm);

void trace(vm_t *vm);
void trace_blacken_objects(stack_toy_t *gray_objects, foobar_object_t *obj);
void trace_mark_object(stack_toy_t *gray_objects, foobar_object_t *obj);

void sweep(vm_t* vm);

void sweep(vm_t* vm);
void vm_collect_garbage(vm_t *vm);