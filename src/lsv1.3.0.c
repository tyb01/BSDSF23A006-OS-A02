#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>   // ioctl, TIOCGWINSZ
#include <unistd.h>
#include "lsv1.3.0.h"

// Function prototypes
void print_down_then_across(char **filenames, int file_count);
void print_horizontal(char **filenames, int file_count);

void do_ls(const char *dirname, int display_mode, int show_all) {
    DIR *dirp;
    struct dirent *entry;
    char **filenames = NULL;
    int count = 0, capacity = 10;
    filenames = malloc(capacity * sizeof(char*));
    if (!filenames) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Open directory
    dirp = opendir(dirname);
    if (!dirp) {
        perror("opendir");
        free(filenames);
        return;
    }

    // Read entries
    while ((entry = readdir(dirp)) != NULL) {
        if (!show_all && entry->d_name[0] == '.')
            continue; // skip hidden files
        if (count >= capacity) {
            capacity *= 2;
            filenames = realloc(filenames, capacity * sizeof(char*));
            if (!filenames) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        filenames[count] = strdup(entry->d_name);
        count++;
    }
    closedir(dirp);

    // Sort filenames
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(filenames[i], filenames[j]) > 0) {
                char *temp = filenames[i];
                filenames[i] = filenames[j];
                filenames[j] = temp;
            }
        }
    }

    // Call the appropriate display function
    if (display_mode == 1) {
        // long listing (not implemented here)
        printf("Long listing not implemented in this example.\n");
    } else if (display_mode == 2) {
        print_horizontal(filenames, count);
    } else {
        print_down_then_across(filenames, count);
    }

    // Free memory
    for (int i = 0; i < count; i++)
        free(filenames[i]);
    free(filenames);
}

// ---------------------- Default "down then across" ------------------------
void print_down_then_across(char **filenames, int file_count) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;

    int max_len = 0;
    for (int i = 0; i < file_count; i++)
        if ((int)strlen(filenames[i]) > max_len)
            max_len = strlen(filenames[i]);

    int col_width = max_len + 2;
    int num_cols = term_width / col_width;
    if (num_cols == 0) num_cols = 1;

    int num_rows = (file_count + num_cols - 1) / num_cols;

    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            int idx = row + col * num_rows;
            if (idx < file_count) {
                printf("%-*s", col_width, filenames[idx]);
            }
        }
        printf("\n");
    }
}

// ---------------------- Horizontal (-x) display ----------------------------
void print_horizontal(char **filenames, int file_count) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;

    int max_len = 0;
    for (int i = 0; i < file_count; i++)
        if ((int)strlen(filenames[i]) > max_len)
            max_len = strlen(filenames[i]);

    int col_width = max_len + 2;
    int pos = 0;

    for (int i = 0; i < file_count; i++) {
        printf("%-*s", col_width, filenames[i]);
        pos += col_width;
        if (pos + col_width > term_width) {
            printf("\n");
            pos = 0;
        }
    }
    if (pos != 0) printf("\n");
}
