#ifndef LSV1_5_0_H
#define LSV1_5_0_H

/* Standard includes used by implementation files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

/* Spacing used between columns */
#define SPACING 2

/* Display modes */
typedef enum {
    DISPLAY_DEFAULT,
    DISPLAY_HORIZONTAL,
    DISPLAY_LONG
} DisplayMode;

/* Public function */
void do_ls(const char *dirname, DisplayMode mode, int show_all);

#endif /* LSV1_5_0_H */
