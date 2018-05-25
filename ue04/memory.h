#ifndef _memory_
#define _memory_

  #include<stdlib.h>
  void memory_init(void);
  void *memory_allocate(size_t);
  void memory_free(void *);
  void memory_print(void);

#endif

