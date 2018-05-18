#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 2048
int main(int argc, char const *argv[])
{
  FILE *file;
  char buffer[BUFF_SIZE];

  if (argc < 2)
  {
    // read from stdin
    file = stdin;
  } else if (argc >= 2)
  {
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
      fprintf(stderr, "Error opening %s.\n", argv[1]);
      return 1;
    }
  }

  while (fgets(buffer, BUFF_SIZE, file) != NULL)
    fprintf(stdout, "%s", buffer);

  fclose(file);
  return 0;
}

