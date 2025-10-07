#include "lsv1.6.0.h"
#include <getopt.h>

int main(int argc, char *argv[]) {
    int long_format = 0, horizontal_format = 0, recursive_flag = 0;
    int opt;

    while ((opt = getopt(argc, argv, "lxR")) != -1) {
        switch (opt) {
        case 'l':
            long_format = 1;
            break;
        case 'x':
            horizontal_format = 1;
            break;
        case 'R':
            recursive_flag = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l] [-x] [-R] [directory...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc)
        do_ls(".", long_format, horizontal_format, recursive_flag);
    else {
        for (int i = optind; i < argc; i++)
            do_ls(argv[i], long_format, horizontal_format, recursive_flag);
    }
    return 0;
}
