
#include <getopt.h>
#include "lsv1.5.0.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    DisplayMode mode = DISPLAY_DEFAULT;
    int show_all = 0;

    /* parse options: -l (long), -x (horizontal), -a (show all) */
    while ((opt = getopt(argc, argv, "lxa")) != -1) {
        switch (opt) {
            case 'l':
                mode = DISPLAY_LONG;
                break;
            case 'x':
                mode = DISPLAY_HORIZONTAL;
                break;
            case 'a':
                show_all = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [-a] [directory]\n", argv[0]);
                return 1;
        }
    }

    const char *dir = ".";
    if (optind < argc) dir = argv[optind];

    do_ls(dir, mode, show_all);
    return 0;
}
