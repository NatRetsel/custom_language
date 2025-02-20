#include <stdio.h>

#include "stack.h"

void stack_push(stack_toy_t *stack, void *obj) {
  /*
    Pushes object into stack.
    if stack is full, double the capacity
    @param stack: stack to push into
    @param obj: object to push into stack
    @return: void
  */
  if (stack->count == stack->capacity) {
    // Double the capacity
    stack->capacity *= 2;
    stack->data = realloc(stack->data, stack->capacity * sizeof(void *));
    if (stack->data == NULL) {
      exit(1);
    }
  }

  stack->data[stack->count] = obj;
  stack->count++;

  return;
}

void *stack_pop(stack_toy_t *stack) {
  /*
    Pops and returns the item at the top of stack.
    @param stack: stack to pop from
    @return: object at the top of stack
    @return: NULL if stack is empty
  */
  if (stack->count == 0) {
    return NULL;
  }

  stack->count--;
  return stack->data[stack->count];
}

void stack_free(stack_toy_t *stack) {
  /*
    Frees the stack, first by freeing the data, then the stack.
    @param stack: stack to free
    @return: void
  */
  if (stack == NULL) {
    return;
  }

  if (stack->data != NULL) {
    free(stack->data);
    stack->data = NULL;
  }

  free(stack);
  stack = NULL;
}

void stack_remove_nulls(stack_toy_t *stack) {
  /*
    Removes all NULL pointers from the stack.
    @param stack: stack to remove NULL pointers from
    @return: void
  */
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

stack_toy_t *stack_new(size_t capacity) {
  /*
    Creates a new stack with the given capacity on the heap.
    @param capacity: capacity of the stack
    @return: stack
    @return: NULL if malloc fails
  */
  stack_toy_t *stack = malloc(sizeof(stack_toy_t));
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