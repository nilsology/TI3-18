#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE 1024
char memory[MEM_SIZE];  // used globally for easy access
struct _memblock *head;

typedef struct _memblock {
  struct _memblock *next;
  struct _memblock *prev;
  bool isFree;
  size_t size;
  void *mAddr;
} _MEMBLOCK;


void memory_init()
{

  // we can use malloc and other lib functions inside of memory_init
  head = malloc(sizeof(struct _memblock));
  head->next = head;
  head->prev = head;
  head->size = MEM_SIZE;
  head->isFree = true;
  head->mAddr = memory;

}


void* memory_allocate(size_t byte_count)
{


  // find large enough block of memory
  struct _memblock *currentBlock = head;
  struct _memblock *fittingBlock = NULL;

  // since head is our initial currentBlock we skip it
  // through the do-while loop

  do
  {
    if (currentBlock->size >= byte_count && currentBlock->isFree)
    {
      fittingBlock = currentBlock; break;
    }

    currentBlock = currentBlock->next;

  } while (currentBlock != head);


  if (!fittingBlock) 
  {
    fprintf(stderr, "err: no (large enough) fitting block of memory found\n");
    return NULL;
  }

  if (fittingBlock->size == byte_count)
  {
    fittingBlock->isFree = false;
    return fittingBlock->mAddr;
  }

  struct _memblock *tmp = head;
  struct _memblock *newBlock;
  memory_init();

  newBlock = head;
  head = tmp;

  newBlock->size = byte_count;
  newBlock->mAddr = fittingBlock->mAddr;
  newBlock->isFree = false;

  fittingBlock->mAddr = (void *) ((size_t) fittingBlock->mAddr + byte_count);
  fittingBlock->size -= byte_count;

  // relink double linked list

  newBlock->next = fittingBlock;
  newBlock->prev = fittingBlock->prev;

  fittingBlock->prev->next = newBlock;
  fittingBlock->prev = newBlock;

  if (newBlock->next == head)
    head = newBlock;

  return newBlock->mAddr;

}

void memory_free(void *pointer)
{

  struct _memblock *currentBlock = head;
  struct _memblock *fittingBlock;

  // since head is our initial currentBlock we skip it
  // through the do-while loop

  do
  {
    if (currentBlock->mAddr == pointer)
    {
      fittingBlock = currentBlock; break;
    }

    currentBlock = currentBlock->next;

  } while (currentBlock != head);

  if (!fittingBlock)
    fprintf(stderr, "err: block of memory not found\n");

  // make block available (again)
  fittingBlock->isFree = true; 

  // combine available blocks of memory close 2 each other
  if (fittingBlock->prev->isFree && fittingBlock->prev != fittingBlock && head != fittingBlock) 
  {
    // add to size
    fittingBlock->prev->size += fittingBlock->size;
    // relink to combine shared space
    fittingBlock->prev->next = fittingBlock->next;
    fittingBlock->next->prev = fittingBlock->prev;
    fittingBlock = fittingBlock->prev;
  }

  if (fittingBlock->next->isFree && fittingBlock->next != fittingBlock && head != fittingBlock->next)
  {
    // add to size
    fittingBlock->size += fittingBlock->next->size;
    // relink to combine shared space
    fittingBlock->next = fittingBlock->next->next;
    fittingBlock->next->prev = fittingBlock;
  }

}

void memory_print()
{


  struct _memblock *currentBlock = head;

  // same loop as usual...
  do
  {
    printf("Memory allocated at: %#18lx, %zu bytes (%s):\n", (uintptr_t)currentBlock->mAddr, (size_t)currentBlock->size, currentBlock->isFree ? "available" : "occupied");
    currentBlock = currentBlock->next;
  } while (currentBlock != head);

}

int main(void)
{

  printf("Initialisiere den Speicher.\n");
  memory_init();
  memory_print();

  printf("Fordere 200 Bytes an.\n");
  int *bereich1 = memory_allocate(200);
  printf("bereich1-Adresse: %p\n", (void *)bereich1);
  memory_print();

  printf("Fordere weitere 212 Bytes an.\n");
  int *bereich2 = memory_allocate(212);
  printf("bereich2-Adresse: %p\n", (void *)bereich2);
  memory_print();

  printf("bereich1 benutzen:\n");
  *bereich1 = 1337;
  bereich1[2] = 0xFEEDF00D;

  printf("bereich2 freigeben.\n");
  memory_free(bereich2);
  memory_print();

  printf("bereich1 freigeben.\n");
  memory_free(bereich1);
  memory_print();

  return 0;

}

