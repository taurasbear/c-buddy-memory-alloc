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
    free_btree->free = true;
    free_btree->left = NULL;
    free_btree->right = NULL;
}

bool is_power_of_two(int number)
{
    if (number <= 0)
    {
        return false;
    }

    return (number & (number - 1)) == 0;
}

bool is_block_split(BlockHeader *block) // is it okay to ask for a pointer here?
{
    return block->left != NULL && block->right != NULL;
}

// bool is_block_empty(BlockHeader *block)
// {
//     return block->size == 0 && block->free == false && block->left == NULL && block->right == NULL;
// }

int round_up_to_power_of_two(int size)
{
    if (size <= 0)
    {
        return 1;
    }

    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    // size |= size >> 32;
    size++;

    return size;
}

int get_split_size(BlockHeader *block)
{
    return (block->size / 2) - sizeof(BlockHeader);
}

void split_block(BlockHeader *block)
{
    if (is_block_split(block))
    {
        return;
    }

    int split_size = block->size / 2 - sizeof(BlockHeader);

    BlockHeader *left = (BlockHeader *)((uint8_t *)block + sizeof(BlockHeader));
    BlockHeader *right = (BlockHeader *)((uint8_t *)left + split_size);

    left->size = split_size;
    left->free = true;
    left->left = NULL;
    left->right = NULL;

    right->size = split_size;
    right->free = true;
    right->left = NULL;
    right->right = NULL;

    block->left = left;
    block->right = right;
}

void *allocate_memory(unsigned int size /*is this okay to do?*/)
{
    // i don't think it makes sense to do this
    // if (!is_power_of_two(size))
    // {
    //     // round to closest power of two
    // }

    if (size <= 0)
    {
        fprintf(stderr, "parameter 'size' must be more than 0");
        return NULL;
    }

    Stack blockStack;
    stack_init(&blockStack, 8);

    // BlockHeader *current = (BlockHeader *)free_btree;
    BlockHeader *current = free_btree;
    while (current)
    {
        if (current->size >= size && is_block_split(current))
        {
            // add left and right to stack
            stack_push(&blockStack, current->right);
            stack_push(&blockStack, current->left);
        }

        if (current->size >= size && !is_block_split(current) && current->free)
        {
            if (get_split_size(current) < size)
            {
                // occupy current block
                current->free = false;
                return current;
            }
            else
            {
                // split
                split_block(current);
                stack_push(&blockStack, current->right);
                stack_push(&blockStack, current->left);
            }
        }
        else
        {
            // pop stack
            current = stack_pop(&blockStack);
            if (!current)
            {
                return NULL;
            }
        }
    }

    return NULL; // Out of memory
}

BlockHeader *get_block_parent(BlockHeader *block)
{
    BlockHeader *temp = (BlockHeader *)((uint8_t *)block - sizeof(BlockHeader));

    if (temp && get_split_size(temp) == block->size)
    {
        return temp;
    }
    else
    {
        // check if it's truly parent by comparing sizes
        temp = (BlockHeader *)((uint8_t *)block - block->size - sizeof(BlockHeader));
        if (get_split_size(temp) == block->size)
        {
            return temp;
        }
    }

    return NULL; // No parent
}

void free_memory(void *mem)
{
    BlockHeader *current = (BlockHeader *)mem;
    current->free = true;

    while (current)
    {
        BlockHeader *parent = get_block_parent(current);

        if (!parent)
        {
            // no parent, block is whole pool
            return;
        }

        if (parent->right->free && parent->left->free)
        {
            parent->right = NULL;
            parent->left = NULL;
            parent->free = true;
        }

        current = parent;
    }
}

// void print_block(BlockHeader block, int count)
// {
//     printf("Count %d: size=%d, free=%d, left=%p, right=%p\n", count, block.size, block.free, (void *)block.left, (void *)block.right);
// }

void print_memory_pool()
{
    Stack blockStack;
    stack_init(&blockStack, 64);

    BlockHeader *current = (BlockHeader *)memory_pool;
    printf("Displaying memory pool\n");

    while (current)
    {
        if (is_block_split(current))
        {
            stack_push(&blockStack, current->right);
            stack_push(&blockStack, current->left);
        }

        printf("Block at %p | Size: %zu | Free: %d\n",
               current, current->size, current->free);
        // current = (BlockHeader *)((uint8_t *)current + sizeof(BlockHeader) + current->size);

        current = stack_pop(&blockStack);
    }
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

    int *arr = allocate_memory(128);

    print_memory_pool();

    printf("Trying to free allocated memory\n");

    free_memory(arr);

    print_memory_pool();

    // Stack stack;
    // BlockHeader block1 = {64, false, NULL, NULL};
    // BlockHeader block2 = {64, true, NULL, NULL};
    // BlockHeader block3 = {128, false, &block1, &block2};
    // BlockHeader block4 = {128, false, NULL, NULL};
    // BlockHeader block5 = {256, false, &block3, &block4};

    // // printf("Block 1: size=%d, free=%d, left=%p, right=%p\n", block1.size, block1.free, (void *)block1.left, (void *)block1.right);
    // // printf("Block 2: size=%d, free=%d, left=%p, right=%p\n", block2.size, block2.free, (void *)block2.left, (void *)block2.right);
    // // printf("Block 3: size=%d, free=%d, left=%p, right=%p\n", block3.size, block3.free, (void *)block3.left, (void *)block3.right);

    // stack_init(&stack, 3);

    // printf("Pushing block1, block2, block3\n");
    // stack_push(&stack, &block1);
    // stack_push(&stack, &block2);
    // stack_push(&stack, &block3);

    // printf("Stack capacity = %d\n", stack.capacity);
    // printf("Stack top = %d\n", stack.top);
    // printf("\n");

    // // printf("Popping block1\n");
    // // BlockHeader pop_block1 = stack_pop(&stack);
    // // print_block(block1, 1);

    // printf("Pushing block4\n");
    // stack_push(&stack, &block4);

    // printf("Stack capacity = %d\n", stack.capacity);
    // printf("Stack top = %d\n", stack.top);
    // printf("\n");

    // printf("Popping block4, block3, block2, block1\n");

    // BlockHeader *pop_block4 = stack_pop(&stack);
    // BlockHeader *pop_block3 = stack_pop(&stack);
    // BlockHeader *pop_block2 = stack_pop(&stack);
    // BlockHeader *pop_block1 = stack_pop(&stack);

    // printf("Printing each popped block\n");

    // print_block(block4, 4);
    // print_block(block3, 3);
    // print_block(block2, 2);
    // print_block(block1, 1);

    // printf("Trying to pop an empty stack\n");

    // BlockHeader *pop_empty = stack_pop(&stack);

    // // if (is_block_empty(pop_empty))
    // if (pop_empty == NULL)
    // {
    //     printf("pop_empty is empty\n");
    // }

    // stack_free(&stack);

    // printf("Testing is_power_of_two method\n");

    // int a = 2;
    // printf("Is %d a number power of two: %d\n", a, is_power_of_two(a));

    // int b = 3;
    // printf("Is %d a number power of two: %d\n", b, is_power_of_two(b));

    // int c = 1024;
    // printf("Is %d a number power of two: %d\n", c, is_power_of_two(c));

    // int d = 1025;
    // printf("Is %d a number power of two: %d\n", d, is_power_of_two(d));

    // printf("Size of int: %d\n", sizeof(int));

    // printf("Testing round_up_to_power_of_two method\n");

    // printf("Round %d to closest number power of two: %d\n", a, round_up_to_power_of_two(a));
    // printf("Round %d to closest number power of two: %d\n", b, round_up_to_power_of_two(b));
    // printf("Round %d to closest number power of two: %d\n", d, round_up_to_power_of_two(d));

    return 0;
}