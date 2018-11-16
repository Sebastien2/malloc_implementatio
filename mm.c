/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Grigrigredinmenufrettin",
    /* First member's full name */
    "Boire",
    /* First member's email address */
    "sebastien.boire@polytechnique.edu",
    /* Second member's full name (leave blank if none) */
    "Dhruv",
    /* Second member's email address (leave blank if none) */
    "dhruv.malik@polytechnique.edu"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))


int sizeOfIncrease=4096; //4Ko pour chaque augmentation de taille de heap
int test=1;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    
    

    int *pointer;
    int sizeHeap;
    if(0)
    {
        sizeHeap=mem_heapsize();
        pointer=mem_heap_lo();
        printf("Current byte size of the heap: %d\n", sizeHeap);
        printf("Current address of the first byte of the heap: %p\n", pointer);
    }
    //we allocate some memory
    int * ptr=mem_sbrk(sizeOfIncrease);
    printf("%c\n", *ptr);
    //we add this as size of first block
    *ptr=sizeOfIncrease+1;
    *(ptr+sizeOfIncrease-1)=sizeOfIncrease+1;
    if(0)
    {
        printf("Apres expansion\n");
        sizeHeap=mem_heapsize();
        pointer=mem_heap_lo();
        printf("Current byte size of the heap: %d\n", sizeHeap);
        printf("Current address of the first byte of the heap: %p\n", pointer);
        pointer=mem_heap_hi();
        printf("Current address of the last byte of the heap: %p\n", pointer);
    }

    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    if(test)
    {
        printf("malloc %d\n", size);
    }
    int basicSolution=0;
    if(basicSolution)
    {
        int newsize = ALIGN(size + SIZE_T_SIZE);
        void *p = mem_sbrk(newsize);
        if (p == (void *)-1)
        return NULL;
        else {
            *(size_t *)p = size;
            return (void *)((char *)p + SIZE_T_SIZE);
        }
    }
    else
    {
        //on commence avec une vraie solution VALABLE
        int newsize = ALIGN(size + 2*SIZE_T_SIZE); // pour mettre les indices
        //printf("Size recherchee: %d\n", newsize);
        int *ptr=mem_heap_lo();
        int tailleBloc;
        int tailleEcrite;
        int blocFree;
        int *endHeap=mem_heap_hi();
        int done=0;
        while(ptr<endHeap && done!=1)
        {
            //on explore le bloc courant
            printf("Position du pointeur courant: %p\n", ptr);
            tailleEcrite=*ptr;
            printf("Taille du bloc: %d\n", tailleEcrite);
            tailleBloc=(tailleEcrite<<1)>>1;
            blocFree=tailleEcrite&1;
            if(blocFree && tailleBloc>=size)
            {
                //on a trouve un bloc qui convient: on le cree
                *ptr=newsize;
                *(ptr+newsize-1)=newsize;
                if(newsize<tailleBloc)
                {
                    *(ptr+newsize)=tailleBloc-newsize+1; //le 1 par ce qu'il est libre
                    *(ptr+tailleBloc-1)=tailleBloc-newsize+1; //le 1 par ce qu'il est libre

                }
                done=1;
                printf("done\n");
                
            }
            ptr+=tailleBloc;
            printf("Position du pointeur courant - fin de boucle: %p\n", ptr);
        }
        if(done)
        {
            return ptr-tailleBloc;
        }
        else
        {
            //we didn't find a free block long enough: we add space into the heap
            return 0;
        }
        
    }
    
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(test)
    {
        printf("free\n");
    }
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if(test)
    {
        printf("realloc\n");
    }
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














