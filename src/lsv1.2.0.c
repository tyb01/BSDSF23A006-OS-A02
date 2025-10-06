#define _GNU_SOURCE   
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include "lsv1.1.0.h"

void print_permissions(mode_t mode) {
    char perms[11];
    perms[0] = (S_ISDIR(mode)) ? 'd' : '-';
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';
    printf("%s ", perms);
}

void do_ls(const char *dir) {
    DIR *dp;
    struct dirent *entry;

    if ((dp = opendir(dir)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] != '.')
            printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dp);
}

void do_ls_long(const char *dir) {
    DIR *dp;
    struct dirent *entry;
    struct stat info;
    struct passwd *pw;
    struct group *gr;

    if ((dp = opendir(dir)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

        if (lstat(path, &info) == -1) {
            perror("lstat");
            continue;
        }

        print_permissions(info.st_mode);
        printf("%3ld ", info.st_nlink);

        pw = getpwuid(info.st_uid);
        gr = getgrgid(info.st_gid);
        printf("%-8s %-8s ", pw->pw_name, gr->gr_name);
        printf("%8ld ", info.st_size);

        char *time_str = ctime(&info.st_mtime);
        time_str[strlen(time_str) - 1] = '\0';
        printf("%s ", time_str);

        printf("%s\n", entry->d_name);
    }
    closedir(dp);
}
