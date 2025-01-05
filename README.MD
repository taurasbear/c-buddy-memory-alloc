# Custom Memory Allocator Inspired by the Buddy System

## A simple educational project using static memory to learn memory allocation concepts, written in C

This project demonstrates a custom memory allocator written in C. It includes `allocate_memory` and `free_memory` functions similar to the standard library's `malloc` and `free`. Before allocating memory, the allocator must be initialized with `initialize_memory_allocator`.

### Key Points

- Memory is allocated in static memory (not the heap)
- This is not a true Buddy System implementation[^1]

The following code example would produce a binary tree structure as shown below:

```c
// in this situation sizeof(BlockHeader) = 24

initialize_memory_allocator();

int *a = allocate_memory(214);
```
![alt text](<buddy-ish system.png>)

The actual block size in this implementation is smaller than expected because each block must include not only the free memory space but also the metadata of the BlockHeader struct[^2]. This metadata is essential for navigating the binary tree.

### Additional Information

[^1]: While this project is inspired by the Buddy System, it does not implement a true Buddy System because a true Buddy System's block size must be a power of two.

[^2]: In order to have a true Buddy System structure, the metadata of the struct would need to be stored elsewhere.