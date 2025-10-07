#ifndef LSV1_6_0_H
#define LSV1_6_0_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define COLOR_RESET "\033[0m"
#define COLOR_DIR "\033[1;34m"
#define COLOR_EXEC "\033[1;32m"
#define COLOR_ARCHIVE "\033[1;31m"
#define COLOR_LINK "\033[1;35m"
#define COLOR_SPECIAL "\033[7m"

void do_ls(const char *dirname, int long_format, int horizontal_format, int recursive_flag);
void display_entry(const char *name, const struct stat *info, int long_format);
void color_print(const char *name, const struct stat *info);
int compare_names(const void *a, const void *b);

#endif
