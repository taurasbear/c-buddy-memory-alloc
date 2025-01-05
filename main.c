#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "block_header.h"
#include "stack.h"

#define MEMORY_POOL_SIZE 1024

static uint8_t memory_pool[MEMORY_POOL_SIZE];

static BlockHeader *free_btree;

bool is_block_split(BlockHeader *block);

int get_split_size(BlockHeader *block);

void print_memory_pool();

void initialize_memory_allocator()
{
    free_btree = (BlockHeader *)memory_pool;
    free_btree->size = MEMORY_POOL_SIZE - sizeof(BlockHeader);
    free_btree->free = true;
    free_btree->left = NULL;
    free_btree->right = NULL;
}

void split_block(BlockHeader *block)
{
    if (is_block_split(block))
    {
        return;
    }

    int split_size = block->size / 2 - sizeof(BlockHeader);

    if (split_size % 2 != 0)
    {
        split_size--; // Intentionally creating external fragmentation to help blocks divide in 2 easier
    }

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

void *allocate_memory(int size)
{
    if (size <= 0)
    {
        fprintf(stderr, "Function parameter 'size' must be greater than 0");
        return NULL;
    }

    Stack blockStack;
    stack_init(&blockStack, 8);

    BlockHeader *current = (BlockHeader *)free_btree;
    while (current)
    {
        if (current->size >= size && is_block_split(current))
        {
            stack_push(&blockStack, current->right);
            stack_push(&blockStack, current->left);
        }

        if (current->size >= size && !is_block_split(current) && current->free)
        {
            int split_size = get_split_size(current);
            if (split_size < 0 || split_size < size)
            {
                current->free = false;
                return (void *)((uint8_t *)current + sizeof(BlockHeader));
            }
            else
            {
                split_block(current);
                stack_push(&blockStack, current->right);
                stack_push(&blockStack, current->left);
            }
        }
        else
        {
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
    BlockHeader *current = (BlockHeader *)((uint8_t *)mem - sizeof(BlockHeader));
    current->free = true;

    while (current)
    {
        BlockHeader *parent = get_block_parent(current);

        if (!parent)
        {
            return;
        }

        if (parent->right->free && !is_block_split(parent->right) && parent->left->free && !is_block_split(parent->left))
        {
            parent->right = NULL;
            parent->left = NULL;
            parent->free = true;
        }

        current = parent;
    }
}

int main()
{
    initialize_memory_allocator();

    int *a = allocate_memory(214);


    // int *a = allocate_memory(sizeof(int) * 5);
    // int *b = allocate_memory(128);
    // int *c = allocate_memory(128);
    // int *d = allocate_memory(9);
    // int *e = allocate_memory(1);

    // print_memory_pool();

    // for (int i = 0; i < 5; i++)
    // {
    //     a[i] = i;
    // }

    // printf("Printing 'a' values\n");

    // for (int i = 0; i < 5; i++)
    // {
    //     printf("Value at index %d: %d\n", i, a[i]);
    // }
    // printf("\n");

    // printf("Trying to free allocated memory\n");

    // free_memory(a);
    // free_memory(b);
    // free_memory(d);
    // free_memory(e);

    // printf("\n");

    print_memory_pool();

    return 0;
}

bool is_block_split(BlockHeader *block)
{
    return block->left != NULL && block->right != NULL;
}

int get_split_size(BlockHeader *block)
{
    int split_size = (block->size / 2) - sizeof(BlockHeader);

    if (split_size <= 1)
    {
        return -1;
    }

    if (split_size % 2 != 0) // Keeping it consistent with the real block sizes
    {
        split_size--;
    }

    return split_size;
}

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

        current = stack_pop(&blockStack);
    }

    printf("\n");
}