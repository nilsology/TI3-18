#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define BUFF_SIZE 2048

int output(FILE *file);

int main(int argc, char const *argv[])
{
  FILE *file;
  pid_t pid;
  int status = 0;

  if (argc < 2)
  {
    // read from stdin
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

    return 0;
  }
}

int output(FILE *file)
{
  char buffer[BUFF_SIZE];

  while (fgets(buffer, BUFF_SIZE, file) != NULL)
    fprintf(stdout, "%s", buffer);

  fclose(file);

  return 0;

}
