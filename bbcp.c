#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>

void crash(char *msg) {
    perror(msg);
    exit(1);
}

int copy(char *src, char *dst) {

    FILE *src_file = fopen(src, "rb");
    if (src_file == NULL)
        crash(src);

    FILE *dst_file = fopen(src, "wb");
    if (dst_file == NULL)
        crash(dst);

    int res;
    char buffer[1];
    while ((res = fread(buffer, 1, 1, src_file)) > 0) {
        fwrite(buffer, 1, 1, dst_file);
    }

    /* Close files using our error handling function */

    if (fclose(src_file) != 0)
        crash(src);
    if (fclose(dst_file) != 0)
        crash(dst);

    if (res == -1) {
        crash("Error writing file");
    }
    if(res == 0)
        return 0;
    else
        return 1;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        crash("Invalid arguments");
    }

    char src[1024];
    strcpy(src,argv[1]);
    char dst[1024];
    strcpy(dst,argv[2]);

    // Validate source


    struct stat src_stat, dst_stat;
    stat(src, &src_stat);
    stat(dst, &dst_stat);

    if(src_stat.st_ino == dst_stat.st_ino)
        crash("Destination is Source");


    // Validating destination

    /* To create a subdirectory if it does not exist */
    mkdir(dirname(strdup(dst)),0777);
    if(S_ISDIR(dst_stat.st_mode)) {
        strcat(dst,"/");
        strcat(dst,basename(strdup(src)));
    }

    return copy(src, dst);
}
