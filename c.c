/*
 * Homework-1 - bbcp.c - bare-bones copy a file
 */

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFERSIZE 16384
#define BUF_SIZE 1048576

/*
 This program demonstrates how the copy command works in NETBSD environment.
 */

int main(int argc, char *argv[]) {
 
  int files[2];
  char fbuf[BUF_SIZE];
  struct stat buf;
  ssize_t content;

  files[0] = open(argv[1], O_RDONLY);

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <SOURCE> <DESTINATION> \n",argv[0]);
    exit(1);
  }
  if ((files[0]) == -1) {
    fprintf(stderr, "Unable to open (permission issues) %s", strerror(errno));
    exit(1);
  };
  
  if (strcmp(argv[1], argv[2]) == 0) {   /* checking if the source file and destination file are the same while copy command is being used*/
    fprintf(stderr, "both files entered are the same\n"); 
    exit(1);
  }
  char* source = argv[1];
  char* destination = argv[2];
  if (access(argv[1], R_OK) != 0) {
    fprintf(stderr, "%s is not readable (access denied)\n", argv[1]);
    exit(1);
  }

  stat(argv[1], &buf);

  if (S_ISDIR(buf.st_mode)) {  /* IS_DIR is a function that checks if the file is a directory or not*/
    printf("Source is a directory\n"); 
    exit(1);
  }

  if (access((dirname(strdup(argv[2]))), W_OK) != 0) {
    fprintf(stderr, "%s is not writable(access denied)",
            dirname(strdup(argv[2])));
    exit(1);
  }

  stat(argv[2], &buf);

  mkdir(dirname(strdup(argv[2])), 0777);  /* Reference: https://stackoverflow.com/questions/5425891/how-do-i-check-if-a-directory-exists-is-dir-file-exists-or-both */
  if (S_ISDIR(buf.st_mode)) {
    strcat(destination, "/");
    strcat(destination, basename(source));
  }

  files[1] = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC,
             S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  while ((content = read(files[0], fbuf, BUF_SIZE)) > 0) {
    if (write(files[1], fbuf, content) != content) {
      exit(1);
    }
  }
  close(files[0]);
  close(files[1]);
  return (0);
}
