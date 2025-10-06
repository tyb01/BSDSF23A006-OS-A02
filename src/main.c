/* src/main.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* for getopt(), optind */
#include <getopt.h>
void do_ls(const char *dirname);
void do_ls_long(const char *dirname);

int main(int argc, char *argv[]) {
    int opt;
    int long_format = 0;

    /* Parse -l option */
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                long_format = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    const char *dirname = (optind < argc) ? argv[optind] : ".";

    if (long_format)
        do_ls_long(dirname);
    else
        do_ls(dirname);

    return 0;
}
