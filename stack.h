#pragma once
#include <stdlib.h>

typedef struct Stack {
  size_t count; //no. of elements
  size_t capacity; //size
  void **data; // no specific data format
} stack_toy_t;

stack_toy_t *stack_new(size_t capacity);

void stack_push(stack_toy_t *stack, void *obj);
void *stack_pop(stack_toy_t *stack);

void stack_free(stack_toy_t *stack);
void stack_remove_nulls(stack_toy_t *stack);