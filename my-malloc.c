// Michael Gallagher
// Assignment 3
// malloc, free, calloc, realloc

// includes
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

// constants
#define ALIGNMENT 16
#define MEMORYCHUNK 4096

// function prototypes
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t element, size_t numBytes);
void *realloc(void *ptr, size_t size);
void *checkFree(int size);
void makeRoom(int size);

//global variables
static int memoryUsed = 0;
static void *heapEnd = NULL; //The break of the heap
static void *heapStart = NULL;

//structs
struct bookKeeping {
    char *nextptr; //points to the next struct
    int isFree; // used as bool to check if memory is free 1 = free, 0 = allocated
    int allocationSize; // stores the number of bytes of the allocation
};

void *malloc(size_t size) {

    if(size == 0) {
        return NULL;
    }

    if(size%16 != 0) {
        int padding = ALIGNMENT-(size%ALIGNMENT);
        size += padding;
    }

    //First run of program, initializes the heap start and end
    if(!heapEnd) {
        struct bookKeeping *allocation;

        heapStart = sbrk(0);
        makeRoom(size + sizeof(*allocation));

        allocation = (struct bookKeeping*) heapStart;
        allocation->isFree = 0;
        allocation->nextptr = ((char*)allocation + sizeof(*allocation) + size);
        allocation->allocationSize = size;
        memoryUsed += size + sizeof(*allocation);
        return ((char*)allocation + sizeof(*allocation));
    }
    //Finds address of bookKeeping struct with free allocation to return to the user
    struct bookKeeping *allocation;
    //makes sure there is enough room in the program for what they are asking
    makeRoom(size + sizeof(*allocation));
    //sets allocation to the bookKeeping info of the available alloc
    allocation = checkFree(size);

    //sets the values of bookKeeping for the allocation about to be given
    //to the user
    if(!allocation->nextptr) {
        allocation->nextptr = ((char*)allocation + sizeof(*allocation) + size);
    }
    allocation->isFree = 0;
    allocation->allocationSize = size;
    memoryUsed += size + sizeof(*allocation);

    return ((char*)allocation + sizeof(*allocation));
}

void free(void *ptr) {
    if(ptr == NULL) {
        return;
    }
    struct bookKeeping *isAllocFree;
    isAllocFree = (struct bookKeeping *)ptr - 1;
    isAllocFree->isFree = 1;
}

void *calloc(size_t element, size_t numBytes) {
    if(element == 0 || numBytes == 0) {
        return NULL;
    }
    int calMem = (element * numBytes);
    void *callocation = malloc(calMem);
    if(callocation) {
        memset(callocation, 0, calMem);
        return callocation;
    }
    else {
        return NULL;
    }
}

void *realloc(void *ptr, size_t size) {
    if(size == 0) {
        free(ptr);
        return ptr;
    }
    //if these cases, return as if call to malloc(size)
    if(ptr == NULL && size >= 0) {
        return malloc(size);
    }
    //else: assume pointer returned by malloc
    struct bookKeeping *reallocation = (struct bookKeeping *)ptr - 1;
    if(reallocation->allocationSize) {
        makeRoom(size);
        if(size < reallocation->allocationSize) {
            free(ptr);
            return malloc(size);
        }
        else if(size > reallocation->allocationSize) {
            void *moveMem = malloc(size);
            if(moveMem) {
                memcpy(moveMem, ptr, reallocation->allocationSize);
                free(ptr);
                return moveMem;
            }
            else {
                return NULL;
            }
        }
        else {
            return ((char *)reallocation + sizeof(*reallocation));
        }
    }
    else {
        return ptr;
    }
}

void *checkFree(int size) {
    struct bookKeeping *checkAlloc;
    checkAlloc = (struct bookKeeping*) heapStart;

    while(checkAlloc->nextptr && (checkAlloc->allocationSize > 0)) {
        if(checkAlloc->isFree == 1) {
            if(size <= checkAlloc->allocationSize) {
                checkAlloc->isFree = 0;
                return checkAlloc;
            }
            else { //allocation is free but not big enough
                checkAlloc = (struct bookKeeping*)checkAlloc->nextptr;
            }
        }
        else { //isFree = 0; allocation is not free
            checkAlloc = (struct bookKeeping*)checkAlloc->nextptr;
        }
    }
    return checkAlloc;
}

void makeRoom(int size) {

    int totalSize = memoryUsed + size;
    int currentHeapSize = (heapEnd) ? ((char*)heapEnd - (char*)heapStart) : 0;

    if(currentHeapSize < totalSize) {
        int needMem = ((size/MEMORYCHUNK)+1);
        int alignedMem = (MEMORYCHUNK * needMem);
        heapEnd = sbrk(alignedMem);
        heapEnd = (char*)heapEnd+alignedMem;
    }
}
