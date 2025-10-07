#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "lsv1.6.0.h"

int compare_names(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s1, s2);
}

void color_print(const char *name, const struct stat *info) {
    if (S_ISDIR(info->st_mode))
        printf(COLOR_DIR "%s" COLOR_RESET, name);
    else if (S_ISLNK(info->st_mode))
        printf(COLOR_LINK "%s" COLOR_RESET, name);
    else if (S_ISCHR(info->st_mode) || S_ISBLK(info->st_mode) ||
             S_ISSOCK(info->st_mode))
        printf(COLOR_SPECIAL "%s" COLOR_RESET, name);
    else if (info->st_mode & S_IXUSR)
        printf(COLOR_EXEC "%s" COLOR_RESET, name);
    else if (strstr(name, ".tar") || strstr(name, ".gz") || strstr(name, ".zip"))
        printf(COLOR_ARCHIVE "%s" COLOR_RESET, name);
    else
        printf("%s", name);
}

void display_entry(const char *name, const struct stat *info, int long_format) {
    if (long_format) {
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&info->st_mtime));
        printf("%c%s %3ld %-8s %-8s %8ld %s ",
               S_ISDIR(info->st_mode) ? 'd' : '-',
               (info->st_mode & S_IRUSR) ? "r" : "-",
               info->st_nlink,
               getpwuid(info->st_uid)->pw_name,
               getgrgid(info->st_gid)->gr_name,
               info->st_size,
               timebuf);
        color_print(name, info);
        printf("\n");
    } else {
        color_print(name, info);
        printf("  ");
    }
}

void do_ls(const char *dirname, int long_format, int horizontal_format, int recursive_flag) {
    DIR *dir_ptr;
    struct dirent *entry;
    struct stat info;
    char path[1024];

    dir_ptr = opendir(dirname);
    if (!dir_ptr) {
        perror(dirname);
        return;
    }

    printf("\n%s:\n", dirname);

    // Collect entries
    char **filenames = NULL;
    size_t count = 0;
    while ((entry = readdir(dir_ptr)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        filenames = realloc(filenames, sizeof(char *) * (count + 1));
        filenames[count++] = strdup(entry->d_name);
    }
    closedir(dir_ptr);

    // Sort alphabetically
    qsort(filenames, count, sizeof(char *), compare_names);

    // Display entries
    for (size_t i = 0; i < count; i++) {
        snprintf(path, sizeof(path), "%s/%s", dirname, filenames[i]);
        if (lstat(path, &info) == -1) {
            perror(path);
            continue;
        }
        display_entry(filenames[i], &info, long_format);
    }
    printf("\n");

    // Recursively list subdirectories if -R flag is set
    if (recursive_flag) {
        for (size_t i = 0; i < count; i++) {
            snprintf(path, sizeof(path), "%s/%s", dirname, filenames[i]);
            if (lstat(path, &info) == -1)
                continue;
            if (S_ISDIR(info.st_mode) && strcmp(filenames[i], ".") != 0 && strcmp(filenames[i], "..") != 0) {
                do_ls(path, long_format, horizontal_format, recursive_flag);
            }
        }
    }

    // Free memory
    for (size_t i = 0; i < count; i++)
        free(filenames[i]);
    free(filenames);
}
