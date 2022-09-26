#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFERSIZE 16384
#define MODE 0666

#ifndef BUF_SIZE
#define BUF_SIZE 1048576
#endif

#ifndef BUF_LIMIT
#define BUF_LIMIT 512
#endif

int main(int argc, char *argv[]) {
  char buffer[BUFFERSIZE];
  int src, dst, in, out;
  char filebuf[BUF_SIZE];
  char bufr[BUF_SIZE];
  struct stat buf;
  ssize_t number;

  src = open(argv[1], O_RDONLY);

  if (!src | argc != 3) {
    fprintf(stderr, "Please specify source-file target-file\n");
    exit(1);
  }

  if (strcmp(argv[1], argv[2]) == 0) {  /* checking if the source file and destination file are the same while copy command is being used*/
    fprintf(stderr, "both files entered are the same\n");
    exit(1);
  }
  if (access(argv[1], R_OK) != 0) {
    fprintf(stderr, "%s is not readable (access denied)\n", argv[1]);
    exit(1);
  }

  stat(argv[1], &buf);

  if (S_ISDIR(buf.st_mode)) {  /* IS_DIR is a function that checks if the file is a directory or not*/
    printf("Source is a directory\n");
    exit(1);
  }

  src = open(argv[1], O_RDONLY);
  if ((src) == -1) {
    fprintf(stderr, "Unable to open %s", strerror(errno));
    exit(1);
  };

  if (access((dirname(strdup(argv[2]))), W_OK) != 0) {
    fprintf(stderr, "%s is not writable(access denied)",
            dirname(strdup(argv[2])));
    exit(1);
  }

  stat(argv[2], &buf);

  mkdir(dirname(strdup(argv[2])), 0777); /* Reference: https://stackoverflow.com/questions/5425891/how-do-i-check-if-a-directory-exists-is-dir-file-exists-or-both */
  if (S_ISDIR(buf.st_mode)) {
    strcat(argv[2], "/");
    strcat(argv[2], basename(strdup(argv[1])));
  }

  dst = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC,
             S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  while ((number = read(src, filebuf, BUF_SIZE)) > 0) {
    if (write(dst, filebuf, number) != number) {
      exit(1);
    }
  }
  close(src);
  close(dst);
  return (0);
}
