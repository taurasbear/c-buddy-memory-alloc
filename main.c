#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_POOL_SIZE 1024

typedef struct BlockHeader
{
    int size;
    bool free;
    struct BlockHeader *left;
    struct BlockHeader *right;

} BlockHeader;

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
    if (size == NULL)
        return 1;

    BlockHeader *current = free_btree;

    

    return NULL;
}

int main()
{
    initialize_memory_allocator();

    printf("Hello World!");

    return 0;
}