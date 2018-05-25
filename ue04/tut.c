#define MEM_SIZE 1024

int main(void)
{
  // reserve space for 500 ints
  int *p = malloc(500, sizeof(int));
  *p = 50;
  *(p + 10) = 13; // greifen auf 11. Zahl in Speicherbereich zu
  p[10] = 14;

  // do stuff with p
  // ...
  // ...

  // when finished
  free(p); // free memory

  struct person {
    int alter;
    char *name;
  }

  struct person *p = malloc(sizeof(struct person));

  // accessing field
  //   (*p).name = ...
  //   p->name = ...


  char memory[MEM_SIZE];

  // ============================================
  // | B | 100 | B | | | | | | ... | | | B |
  //   _     
  // ============================================

  sb *b = (struct block *) (memory + 100 + sizeof(struct block));
  b->free = 0;

  struct block {

    struct block *next;
    int isFree; 
    //      size_t size;
    //      int memoryAddress;
  }


}
