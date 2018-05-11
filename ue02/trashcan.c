#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define TRASH_PATH ".ti3_trashcan"

static int maketrash(void);
static int list(void);
static int copy(char *sourcename, char *targetname);
static int move(char *sourcename, char *targetname);

int main(int argc, char *argv[])
{

  char *mode = argv[1];

  // argument handling (only one argument provided)
  if (argc == 2 && strncmp(mode, "-l", 2) == 0)
  {
    list();
    return 0;
  }

  char *file = argv[2];

  /*
   * buffer to concatenate filename
   */
  size_t buffer_size = strlen(TRASH_PATH) + strlen(file) + 1;
  char buffer[buffer_size];
  strcpy(buffer, TRASH_PATH);
  strcat(buffer, "/");
  strcat(buffer, file);

  maketrash();

  // argument handling
  if (strncmp(mode, "-d", 2) == 0)
    move(file, buffer);

  if (strncmp(mode, "-r", 2) == 0)
    move(buffer, file);

  if (strncmp(mode, "-f", 2) == 0)
    unlink(buffer);

  if (strncmp(mode, "-l", 2) == 0)
    list(); 

  return 0;
}

int maketrash(void)
{
  struct stat st = {0};
  // create directory if not yet exists
  if (stat(TRASH_PATH, &st) == -1) {
    mkdir(".ti3_trashcan", 0777);
  }
  return 0;
}

int copy(char *sourcename, char *targetname)
{
  // no error, but file exists
  if (access(targetname, F_OK) != -1)
    return 0;

  int source_fd = open(sourcename, O_RDONLY);
  // Return with error, since file couldn't be opened
  if (source_fd == -1)
    return -1;

  int target_fd = open(targetname, O_WRONLY | O_CREAT | O_EXCL, 0777);
  // Return with error, since file couldn't be opened
  if (target_fd == -1)
    return -1;

  // size of source_fd
  off_t fd_size = lseek(source_fd, 0, SEEK_END);

  // lseek error handling, since needed for buffer
  if (fd_size == -1)
    return 1;

  // create buffer
  char buffer[fd_size];

  // read from buffer up to fd_size
  read(source_fd, buffer, fd_size);

  // write from buffer up to fd_size
  write(target_fd, &buffer, fd_size);
  close(source_fd);
  close(target_fd);

  return 0;
}

// copy + unlink (delete)
int move(char *sourcename, char *targetname)
{
  // ensure copying without error
  if (copy(sourcename, targetname) == -1)
    return -1;

  unlink(sourcename);
  return 0;
}

int list(void)
{

  DIR *d;
  struct dirent *dir;
  d = opendir(TRASH_PATH);

  // NULL returned on error
  if (d == NULL)
    return -1;

  while ((dir = readdir(d)) != NULL)
  {
    char *fn = dir->d_name;
    // skip links to parent and current folder
    if (strncmp(fn, ".", 1) == 0 || strncmp(fn, "..", 2) == 0) continue;

    write(1, fn, strlen(fn)); // write filename
    write(1, "\n", 1); // line break
  }
  closedir(d);

  return 0;
}
