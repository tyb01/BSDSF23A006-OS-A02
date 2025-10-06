/* src/lsv1.0.0.c -- simple listing function (no -l) */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

void do_ls(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.')
            printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}
