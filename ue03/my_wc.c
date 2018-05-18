#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


int main(int argc, char const *argv[])
{
  FILE* file;
  char character;

  uint64_t bytes = 0; // B - etriebs
  uint64_t words = 0; // W - irtschafts
  uint64_t lines = 0; // L - ehre

  if (argc < 2)
    file = stdin;
  else if (argc >= 2)
  {
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
      fprintf(stderr, "Error opening %s.\n", argv[1]);
      return 1;
    }
  }

  while ((character = fgetc(file)) != EOF)
  {
    bytes++; // increase for every byte read
    if (character == '\n')
      lines++; // increase lines for every linebreak

    // increase words for every delimiter
    switch (character)
    {
      case ' ' : words++; break;
      case '\t': words++; break;
      case '\n': words++; break;
      case '\r': words++; break;
      default  : break;
    }
  }

  // since uint64_t is used for counters
  fprintf(stdout,"words: %"PRIu64" \tlines: %"PRIu64" \t bytes: %"PRIu64"\n", words, lines, bytes);

  fclose(file);
  return 0;
}

