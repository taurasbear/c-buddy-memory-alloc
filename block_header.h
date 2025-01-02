#ifndef BLOCK_HEADER_H
#define BLOCK_HEADER_H

#include <stdbool.h>

typedef struct BlockHeader
{
    int size;
    bool free;
    struct BlockHeader *left;
    struct BlockHeader *right;

} BlockHeader;

#endif