#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // getopt
#include <dirent.h>
#include <string.h>
#include "lsv1.3.0.h"

int main(int argc, char *argv[]) {
    int opt;
    int display_mode = 0; // 0 = default down-then-across, 1 = long, 2 = horizontal
    int show_all = 0;     // -a flag
    char *directory = "."; // default directory

    // Parse options
    while ((opt = getopt(argc, argv, "lxa")) != -1) {
        switch (opt) {
            case 'l':
                display_mode = 1;
                break;
            case 'x':
                display_mode = 2;
                break;
            case 'a':
                show_all = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [-a] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if directory is specified
    if (optind < argc) {
        directory = argv[optind];
    }

    // Call do_ls with flags
    do_ls(directory, display_mode, show_all);

    return 0;
}
