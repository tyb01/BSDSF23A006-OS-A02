#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function prototypes
void do_ls(const char *dir);
void do_ls_long(const char *dir);

int main(int argc, char *argv[]) {
    int opt;
    int long_list = 0;
    char *path = ".";  // default directory

    // Parse command-line arguments using getopt
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                long_list = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        path = argv[optind];  // if user gives directory name
    }

    if (long_list)
        do_ls_long(path);
    else
        do_ls(path);

    return 0;
}
