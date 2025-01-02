typedef struct BlockHeader
{
    int size;
    bool free;
    struct BlockHeader *left;
    struct BlockHeader *right;

} BlockHeader;