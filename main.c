#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "block_header.h"
#include "stack.h"

#define MEMORY_POOL_SIZE 1024

static uint8_t memory_pool[MEMORY_POOL_SIZE];

static BlockHeader *free_btree;

void initialize_memory_allocator()
{
    free_btree = (BlockHeader *)memory_pool;
    free_btree->size = MEMORY_POOL_SIZE - sizeof(BlockHeader);
    free_btree->left = NULL;
    free_btree->right = NULL;
}

int allocate_memory(int size)
{
    if (size <= 0 || size)
    {
        return -1;
    }

    BlockHeader *current = free_btree;

    return 1;
}

void print_block(BlockHeader block, int count)
{
    printf("Count %d: size=%d, free=%d, left=%p, right=%p\n", count, block.size, block.free, (void *)block.left, (void *)block.right);
}

bool is_block_empty(BlockHeader block)
{
    return block.size == 0 && block.free == false && block.left == NULL && block.right == NULL;
}

// int get_bit(int number)

bool is_power_of_two(int number)
{
    if (number <= 0)
    {
        return false;
    }

    return (number & (number - 1)) == 0;
}

// bool is_power_of_two(int number)
// {
//     int on_count = 0;
//     for (int i = 0; i < sizeof(int); i++)
//     {
//         int first_bit = number & 1;
//         if (first_bit == 1)
//         {
//             on_count++;
//             if (on_count > 1)
//             {
//                 return false;
//             }
//         }
//         number >> 1;
//     }

//     return true;
// }

int main()
{
    initialize_memory_allocator();

    printf("Hello World!\n");

    Stack stack;
    BlockHeader block1 = {64, false, NULL, NULL};
    BlockHeader block2 = {64, true, NULL, NULL};
    BlockHeader block3 = {128, false, &block1, &block2};
    BlockHeader block4 = {128, false, NULL, NULL};
    BlockHeader block5 = {256, false, &block3, &block4};

    // printf("Block 1: size=%d, free=%d, left=%p, right=%p\n", block1.size, block1.free, (void *)block1.left, (void *)block1.right);
    // printf("Block 2: size=%d, free=%d, left=%p, right=%p\n", block2.size, block2.free, (void *)block2.left, (void *)block2.right);
    // printf("Block 3: size=%d, free=%d, left=%p, right=%p\n", block3.size, block3.free, (void *)block3.left, (void *)block3.right);

    stack_init(&stack, 3);

    printf("Pushing block1, block2, block3\n");
    stack_push(&stack, block1);
    stack_push(&stack, block2);
    stack_push(&stack, block3);

    printf("Stack capacity = %d\n", stack.capacity);
    printf("Stack top = %d\n", stack.top);
    printf("\n");

    // printf("Popping block1\n");
    // BlockHeader pop_block1 = stack_pop(&stack);
    // print_block(block1, 1);

    printf("Pushing block4\n");
    stack_push(&stack, block4);

    printf("Stack capacity = %d\n", stack.capacity);
    printf("Stack top = %d\n", stack.top);
    printf("\n");

    printf("Popping block4, block3, block2, block1\n");

    BlockHeader pop_block4 = stack_pop(&stack);
    BlockHeader pop_block3 = stack_pop(&stack);
    BlockHeader pop_block2 = stack_pop(&stack);
    BlockHeader pop_block1 = stack_pop(&stack);

    printf("Printing each popped block\n");

    print_block(block4, 4);
    print_block(block3, 3);
    print_block(block2, 2);
    print_block(block1, 1);

    printf("Trying to pop an empty stack\n");

    BlockHeader pop_empty = stack_pop(&stack);

    if (is_block_empty(pop_empty))
    {
        printf("pop_empty is empty");
    }

    stack_free(&stack);

    printf("Testing is_power_of_two method\n");

    int a = 2;
    printf("Is %d a number power of two: %d\n", a, is_power_of_two(a));

    int b = 3;
    printf("Is %d a number power of two: %d\n", b, is_power_of_two(b));

    int c = 1024;
    printf("Is %d a number power of two: %d\n", c, is_power_of_two(c));

    int d = 1025;
    printf("Is %d a number power of two: %d\n", d, is_power_of_two(d));

    printf("Size of int: %d", sizeof(int));

    return 0;
}