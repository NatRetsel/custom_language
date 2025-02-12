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