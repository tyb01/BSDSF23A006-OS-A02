/* src/lsv1.1.0.c -- long listing (-l) implementation */
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

/* Print permission bits and file type */
void print_permissions(mode_t mode) {
    char perms[11];
    perms[0] = S_ISDIR(mode)  ? 'd' :
               S_ISLNK(mode)  ? 'l' :
               S_ISCHR(mode)  ? 'c' :
               S_ISBLK(mode)  ? 'b' :
               S_ISFIFO(mode) ? 'p' :
               S_ISSOCK(mode) ? 's' : '-';

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

    printf("%s", perms);
}

/* Long listing function */
void do_ls_long(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat info;
    char fullpath[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        /* skip hidden files */
        if (entry->d_name[0] == '.')
            continue;

        /* build path */
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirname, entry->d_name);

        if (lstat(fullpath, &info) == -1) {
            perror("lstat");
            continue;
        }

        /* permissions */
        print_permissions(info.st_mode);

        /* links */
        printf(" %2ld", (long)info.st_nlink);

        /* owner and group */
        struct passwd *pw = getpwuid(info.st_uid);
        struct group  *gr = getgrgid(info.st_gid);
        printf(" %-8s %-8s", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");

        /* size */
        printf(" %8ld", (long)info.st_size);

        /* time */
        char tbuf[64];
        strftime(tbuf, sizeof(tbuf), "%b %e %H:%M", localtime(&info.st_mtime));
        printf(" %s", tbuf);

        /* name */
        printf(" %s", entry->d_name);

        /* symlink target if applicable */
        if (S_ISLNK(info.st_mode)) {
            char target[PATH_MAX];
            ssize_t len = readlink(fullpath, target, sizeof(target) - 1);
            if (len != -1) {
                target[len] = '\0';
                printf(" -> %s", target);
            }
        }

        printf("\n");
    }

    closedir(dir);
}
