#include "stack.h"
#include "block_header.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void stack_init(Stack *stack, int capacity)
{
    stack->capacity = capacity;
    stack->top = -1;
    stack->data = (BlockHeader *)malloc(stack->capacity * sizeof(BlockHeader));
}

bool stack_is_full(Stack *stack)
{
    return stack->top + 1 == stack->capacity;
}

bool stack_is_empty(Stack *stack)
{
    return stack->top == -1;
}

void stack_push(Stack *stack, BlockHeader element)
{
    if (stack_is_full(stack))
    {
        stack->capacity *= 2;
        stack->data = (BlockHeader *)realloc(&stack->data, stack->capacity * sizeof(BlockHeader));
    }

    stack->data[++stack->top] = element;
}

BlockHeader stack_pop(Stack *stack)
{
    if (stack_is_empty(stack))
    {
        fprintf(stderr, "Stack underflow\n");
        BlockHeader empty = {0};
        return empty;
    }

    return stack->data[stack->top--];
}

void stack_free(Stack *stack)
{
    BlockHeader empty = {0};
    free(stack->data);
    stack->top = -1;
    stack->capacity = 0;
}