#include <getopt.h>
#include "lsv1.4.0.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    DisplayMode mode = DISPLAY_DEFAULT;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "lx")) != -1) {
        switch (opt) {
            case 'l':
                mode = DISPLAY_LONG;
                break;
            case 'x':
                mode = DISPLAY_HORIZONTAL;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [directory]\n", argv[0]);
                return 1;
        }
    }

    // Determine directory
    const char *dir = ".";
    if (optind < argc) dir = argv[optind];

    // Call ls function
    do_ls(dir, mode);

    return 0;
}
