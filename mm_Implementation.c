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
int sizeOfInt=sizeof(int);


//prints a description of the heap
void toString()
{
	//printf("One byte is:%d\n", SIZE_T_SIZE);
    printf("Description of the heap:\n");
    void *currentPos=mem_heap_lo();
    int heapsize;
    void *endHeap=mem_heap_hi();
    int size, size2;
    void * endBlock;
    int * ptr;
    heapsize=mem_heapsize();
    printf("Heapsize: %x\n", heapsize);
    endBlock=mem_heap_lo();
    printf("Pointeur sur le debut de heap: %p\n", endBlock);
    endBlock=mem_heap_hi();
    printf("Pointeur sur la fin de heap: %p\n", endBlock);
    while(currentPos<=endHeap)
    {
        printf("-------------\n");
        ptr=(int *)(currentPos);
        size=*ptr;
        
        printf("%p : %x\n", currentPos, size);
        if(size&1)
        {
            printf("free ");
        }
        else
        {
            printf("occupied ");
        }
        size2=(size>>1)<<1;
        
        
        printf(", Size: %x\n", size2);
        
        endBlock=currentPos+(size2);
        ptr=(int *)(endBlock)-SIZE_T_SIZE/sizeOfInt;  //we go back of 2 for alignment reason
        size=*ptr;
        printf("%p : %x\n", ptr, size);
        currentPos+=size2;
        if(size2==0)
        {
            //there-s a problem : no need to keep going
            break;
        }
    }
    printf("-------------\n");
}




/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{

	//printf("SIZE_T_SIZE: %x\n", SIZE_T_SIZE);
    int * ptr2;
    
    if(0)
    {
        int * pointer;
        int sizeHeap;
        sizeHeap=mem_heapsize();
        pointer=mem_heap_lo();
        printf("Current byte size of the heap: %d\n", sizeHeap);
        printf("Beginnig of the heap: %p\n", pointer);
    }
    
    //we initialize with a certain size
    int size=ALIGN((sizeOfIncrease)*sizeof(int));
    if(test)
    {
    	printf("On ajoute %x bytes à la taille\n", size);
    
    }
    void * ptr=mem_sbrk(size);
    ptr2=(int *)(ptr);
    
    
    //we add this as size of first block
    *ptr2=size+1;
    ptr2=(int *)(ptr+size);
    *(ptr2-SIZE_T_SIZE/sizeOfInt)=size+1; //1: it's free
    ptr=ptr+(size-1);
    //printf("Fin du bloc: %p\n", ptr);
    
    //toString();
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
        printf("\nmalloc %x\n", size);
    }
    toString();
    int basicSolution=0;
    if(basicSolution)  //just to be careful, we didn't remove this code, but it is useless
    {
        int newsize = ALIGN(size + 2*SIZE_T_SIZE);
        void *p = mem_sbrk(newsize);
        if (p == (void *)-1)
        return NULL;
        else 
        {
            *(size_t *)p = size;
            return (void *)((char *)p + SIZE_T_SIZE);
        }
    }
    else
    {
        //on commence avec une vraie solution VALABLE
        int newsize = ALIGN(size + 2*SIZE_T_SIZE); // pour mettre les indices
        printf("Size recherchee: 0x%x\n", newsize);
        void *ptr=mem_heap_lo();
        int tailleBloc=0;
        int tailleEcrite;
        int blocFree;
        void *endHeap=mem_heap_hi();
        int done=0;
        int * ptr2;
        while(ptr<endHeap && done!=1)
        {
            //on explore le bloc courant
            //printf("Position du pointeur courant: %p\n", ptr);
            ptr2=(int *)(ptr);
            tailleEcrite=*ptr2;
            //printf("Taille du bloc: %d\n", tailleEcrite);
            tailleBloc=(tailleEcrite>>1)<<1;
            if(tailleBloc==0)
            {
            	printf("probleme de taille de bloc nul\n");
                return 0;
            }
            blocFree=tailleEcrite&1;
            if(blocFree && tailleBloc>=newsize)
            {
                //we have found a correct block: we set this space to be occupied
                ptr2=(int *)(ptr);
                *ptr2=newsize;
                
                ptr2=(int *)(ptr+newsize);
                *(ptr2-SIZE_T_SIZE/sizeOfInt)=newsize;
                
                if(newsize<tailleBloc) //there's still a piece of free memory after that
                {
                	//printf("Sizes: %x, %x\n", tailleBloc, newsize);
                    *ptr2=tailleBloc-newsize+1; //+1 beacause it's free
                    //printf("Value of the remaining block: %x", *ptr2);
                    ptr2=(int *)(ptr+tailleBloc);
                    //printf("Ending position: %p\n", ptr2);
                    *(ptr2-SIZE_T_SIZE/sizeOfInt)=tailleBloc-newsize+1; //le 1 par ce qu'il est libre
					//printf("Analysing the heap after change of the value at top of the heap\n");
                	//toString();
                }
                done=1;
                //printf("done\n");
                
            }
            ptr+=tailleBloc;
            //printf("Position du pointeur courant - fin de boucle: %p\n", ptr);
        }
        if(done)
        {
        	toString();
            return ptr-tailleBloc;
        }
        else
        {
        	
            //we didn't find a free block long enough: we add space into the heap
            int * endOfHeap=mem_heap_hi();
            int * newPos=mem_sbrk(newsize); //we make sure there is enough space
            if(test)
            {
                printf("Former end of heap: %p\n", endOfHeap);
                printf("Beginning of the new block of the heap: %p\n", newPos);
            }

            //ptr is a pointer to the last byte of the former heap
            ptr=ptr-tailleBloc;
            ptr2=(int *)(ptr);
            *ptr2=*ptr2+newsize;
            ptr=ptr+tailleBloc;
            ptr2=(int *)(ptr);
            *ptr2=0;
            ptr=ptr+newsize+tailleBloc;
            ptr2=(int *)(ptr);
            *(ptr2)=*((int *)(ptr-tailleBloc));
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
    return 0;
    
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





















