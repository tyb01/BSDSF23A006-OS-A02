
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define _POSIX_C_SOURCE 200809L   /* expose strdup on some systems */

#include "lsv1.5.0.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

/* ANSI color definitions */
#define COLOR_RESET   "\033[0m"
#define COLOR_BLUE    "\033[0;34m"  /* directories */
#define COLOR_GREEN   "\033[0;32m"  /* executables */
#define COLOR_RED     "\033[0;31m"  /* archives */
#define COLOR_MAGENTA "\033[0;35m"  /* symbolic links */
#define COLOR_REVERSE "\033[7m"     /* special files */

/* Comparison for qsort */
static int compare_filenames(const void *a, const void *b) {
    const char *s1 = *(const char * const *)a;
    const char *s2 = *(const char * const *)b;
    return strcmp(s1, s2);
}

/* Helper: get terminal width (fallback to 80) */
static int get_term_width(void) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return 80;
    }
    return (int)ws.ws_col;
}

/* Print a filename with color according to its type.
   If stdout isn't a TTY, prints without color. */
static void print_colored_name(const char *dirpath, const char *name) {
    /* If not a tty, just print plain name */
    if (!isatty(STDOUT_FILENO)) {
        printf("%s", name);
        return;
    }

    char fullpath[PATH_MAX];
    struct stat st;

    /* Construct full path safely */
    if (snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, name) >= (int)sizeof(fullpath)) {
        /* path too long; fallback to plain print */
        printf("%s", name);
        return;
    }

    /* Use lstat to detect symlinks */
    if (lstat(fullpath, &st) == -1) {
        /* If stat fails, just print name */
        printf("%s", name);
        return;
    }

    /* Choose color/style */
    if (S_ISDIR(st.st_mode)) {
        printf(COLOR_BLUE "%s" COLOR_RESET, name);
    } else if (S_ISLNK(st.st_mode)) {
        printf(COLOR_MAGENTA "%s" COLOR_RESET, name);
    } else if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) || S_ISSOCK(st.st_mode) || S_ISFIFO(st.st_mode)) {
        printf(COLOR_REVERSE "%s" COLOR_RESET, name);
    } else if (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        /* executable */
        printf(COLOR_GREEN "%s" COLOR_RESET, name);
    } else if (strstr(name, ".tar") || strstr(name, ".gz") || strstr(name, ".zip")) {
        printf(COLOR_RED "%s" COLOR_RESET, name);
    } else {
        printf("%s", name);
    }
}

/* Vertical "down then across" display (columnar) */
static void print_vertical(const char *dirpath, char **filenames, int count) {
    int termw = get_term_width();
    int maxlen = 0;
    for (int i = 0; i < count; ++i) {
        int l = (int)strlen(filenames[i]);
        if (l > maxlen) maxlen = l;
    }
    int colw = maxlen + SPACING;
    if (colw <= 0) colw = 1;
    int cols = termw / colw;
    if (cols < 1) cols = 1;
    int rows = (count + cols - 1) / cols;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int idx = c * rows + r;
            if (idx >= count) continue;
            /* print colored name, then pad */
            print_colored_name(dirpath, filenames[idx]);
            if (c != cols - 1) {
                /* compute padding width based on visible name length (we use strlen; ANSI doesn't count) */
                int pad = colw - (int)strlen(filenames[idx]);
                if (pad < 1) pad = 1;
                for (int p = 0; p < pad; ++p) putchar(' ');
            }
        }
        putchar('\n');
    }
}

/* Horizontal (-x) display: left-to-right, wrapping by terminal width */
static void print_horizontal(const char *dirpath, char **filenames, int count) {
    int termw = get_term_width();
    int maxlen = 0;
    for (int i = 0; i < count; ++i) {
        int l = (int)strlen(filenames[i]);
        if (l > maxlen) maxlen = l;
    }
    int colw = maxlen + SPACING;
    if (colw <= 0) colw = 1;

    int pos = 0;
    for (int i = 0; i < count; ++i) {
        int name_len = (int)strlen(filenames[i]);
        /* if next column doesn't fit, newline */
        if (pos != 0 && pos + colw > termw) {
            putchar('\n');
            pos = 0;
        }
        /* print colored name */
        print_colored_name(dirpath, filenames[i]);
        /* pad to column width */
        int pad = colw - name_len;
        if (pad < 1) pad = 1;
        for (int p = 0; p < pad; ++p) putchar(' ');
        pos += colw;
    }
    if (pos != 0) putchar('\n');
}

/* Simple long listing — prints colored name plus basic type marker (as simple example) */
static void print_long(const char *dirpath, char **filenames, int count) {
    struct stat st;
    for (int i = 0; i < count; ++i) {
        char fullpath[PATH_MAX];
        if (snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, filenames[i]) >= (int)sizeof(fullpath)) {
            printf("%s\n", filenames[i]);
            continue;
        }
        if (lstat(fullpath, &st) == -1) {
            printf("%s\n", filenames[i]);
            continue;
        }
        /* for long listing we simply show permissions, size and name (basic) */
        /* Permissions: very simple rwx display for owner only (for brevity) */
        putchar( (S_ISDIR(st.st_mode)) ? 'd' : '-' );
        putchar( (st.st_mode & S_IRUSR) ? 'r' : '-' );
        putchar( (st.st_mode & S_IWUSR) ? 'w' : '-' );
        putchar( (st.st_mode & S_IXUSR) ? 'x' : '-' );
        printf(" %6lld ", (long long)st.st_size);
        print_colored_name(dirpath, filenames[i]);
        putchar('\n');
    }
}

/* Core function: read entries, sort, dispatch to display functions */
void do_ls(const char *dirname, DisplayMode mode, int show_all) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror(dirname);
        return;
    }

    struct dirent *entry;
    char **names = NULL;
    int count = 0, cap = 16;
    names = malloc(cap * sizeof(char *));
    if (!names) {
        perror("malloc");
        closedir(dir);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        /* skip hidden unless show_all */
        if (!show_all && entry->d_name[0] == '.') continue;

        if (count >= cap) {
            cap *= 2;
            char **tmp = realloc(names, cap * sizeof(char *));
            if (!tmp) {
                perror("realloc");
                /* free what we have */
                for (int i = 0; i < count; ++i) free(names[i]);
                free(names);
                closedir(dir);
                return;
            }
            names = tmp;
        }
        names[count] = strdup(entry->d_name);
        if (!names[count]) {
            perror("strdup");
            for (int i = 0; i < count; ++i) free(names[i]);
            free(names);
            closedir(dir);
            return;
        }
        ++count;
    }
    closedir(dir);

    /* sort alphabetically */
    if (count > 1) qsort(names, count, sizeof(char *), compare_filenames);

    /* print header for recursive features if needed (not used here) */
    /* dispatch to selected display mode */
    if (mode == DISPLAY_HORIZONTAL) {
        print_horizontal(dirname, names, count);
    } else if (mode == DISPLAY_LONG) {
        print_long(dirname, names, count);
    } else {
        print_vertical(dirname, names, count);
    }

    /* free memory */
    for (int i = 0; i < count; ++i) free(names[i]);
    free(names);
}
