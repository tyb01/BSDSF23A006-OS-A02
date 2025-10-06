#ifndef LSV1_4_0_H
#define LSV1_4_0_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define SPACING 2  // spaces between columns

// Display mode enum
typedef enum {
    DISPLAY_DEFAULT,
    DISPLAY_HORIZONTAL,
    DISPLAY_LONG
} DisplayMode;

// Function declarations
void do_ls(const char *dirname, DisplayMode mode);
void print_vertical(char **filenames, int count);
void print_horizontal(char **filenames, int count);
void print_long(char **filenames, int count);
int compare_filenames(const void *a, const void *b);

#endif
