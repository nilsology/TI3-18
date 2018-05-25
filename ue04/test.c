#include "memory.h"
#include<stdio.h>

int main() {
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

