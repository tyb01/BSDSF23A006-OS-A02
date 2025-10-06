#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "lsv1.4.0.h"

// Comparison function for qsort
int compare_filenames(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
}

// Default vertical display: down then across
void print_vertical(char **filenames, int count) {
    struct winsize w;
    int maxlen = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(filenames[i]);
        if (len > maxlen) maxlen = len;
    }
    int term_width = 80;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        term_width = w.ws_col;
    }
    int cols = term_width / (maxlen + SPACING);
    if (cols == 0) cols = 1;
    int rows = (count + cols - 1) / cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int idx = r + c * rows;
            if (idx < count) {
                printf("%-*s", maxlen + SPACING, filenames[idx]);
            }
        }
        printf("\n");
    }
}

// Horizontal display: left to right
void print_horizontal(char **filenames, int count) {
    struct winsize w;
    int maxlen = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(filenames[i]);
        if (len > maxlen) maxlen = len;
    }
    int term_width = 80;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        term_width = w.ws_col;
    }

    int current_width = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(filenames[i]);
        if (current_width + len + SPACING > term_width) {
            printf("\n");
            current_width = 0;
        }
        printf("%-*s", maxlen + SPACING, filenames[i]);
        current_width += len + SPACING;
    }
    printf("\n");
}

// Simple long listing (filename only for now)
void print_long(char **filenames, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s\n", filenames[i]);
    }
}

// Core ls function
void do_ls(const char *dirname, DisplayMode mode) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char **filenames = NULL;
    int count = 0;
    int capacity = 10;
    filenames = malloc(capacity * sizeof(char *));
    if (!filenames) {
        perror("malloc");
        closedir(dir);
        return;
    }

    // Read all filenames
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;  // skip hidden files
        if (count >= capacity) {
            capacity *= 2;
            filenames = realloc(filenames, capacity * sizeof(char *));
            if (!filenames) {
                perror("realloc");
                closedir(dir);
                return;
            }
        }
        filenames[count] = strdup(entry->d_name);
        if (!filenames[count]) {
            perror("strdup");
            closedir(dir);
            return;
        }
        count++;
    }
    closedir(dir);

    // Sort alphabetically
    qsort(filenames, count, sizeof(char *), compare_filenames);

    // Call appropriate display function
    if (mode == DISPLAY_HORIZONTAL) {
        print_horizontal(filenames, count);
    } else if (mode == DISPLAY_LONG) {
        print_long(filenames, count);
    } else {
        print_vertical(filenames, count);
    }

    // Free memory
    for (int i = 0; i < count; i++) free(filenames[i]);
    free(filenames);
}
