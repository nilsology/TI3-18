#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<inttypes.h>
#include<sys/types.h>
#include<sys/wait.h>

int output(FILE *file);

int main(int argc, char const *argv[])
{
  FILE *file;
  pid_t pid;
  int status = 0;

  if (argc < 2)
  {
    file = stdin;
    output(file);
  }
  else if (argc >= 2)
  {
    int c = 1;
    while (c <= argc - 1) {
      file = fopen(argv[c], "r");
      if (file == NULL)
      {
        fprintf(stderr, "Error opening %s.\n", argv[c]);
        return 1;
      }

      if ((pid = fork()) < 0)
      {
        fprintf(stderr, "Error forking child process\n");
        return 1; 
      }
      else if (pid == 0)
      {
        if (output(file) < 0)
        {
        fprintf(stderr, "Error executing child process\n");
        return 1;
        }
      }
      else {
        while(wait(&status) != pid);
        exit(0);
      }
      c++;
    }

  }

  return 0;
}

int output(FILE *file)
{

  char character;
  uint64_t bytes = 0; // B - etriebs
  uint64_t words = 0; // W - irtschafts
  uint64_t lines = 0; // L - ehre

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
