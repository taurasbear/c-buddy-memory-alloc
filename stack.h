#ifndef STACK_H
#define STACK_H

#include "block_header.h"

typedef struct Stack
{
    BlockHeader *data;
    int top;
    int capacity;

} Stack;

void stack_init(Stack *stack, int capacity);
void stack_push(Stack *stack, BlockHeader element);
BlockHeader stack_pop(Stack *stack);
void stack_free(Stack *stack);

#endif