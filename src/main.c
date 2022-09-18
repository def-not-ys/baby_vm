#include <unistd.h>
#include "baby_vm.h"

// flags 
int file_flg = 0;
int arg_flg = 0;
int verbose_flg = 0;
int help_flg = 0;

int main(int argc, char* argv[])
{
    int opt = 0;
    char* file_path = NULL;
    Arguments args = {0};

    /*
     *  baby_vm should take (at least) one argument:
     *  path and name of the assembly.txt file
     *  arguments supplied to the program to be executed (OPTIONAL)
     */

    int err_flg = 1;
    while ((opt = getopt(argc, argv, ":f:a:vh")) != -1) {
        printf("%c\n", opt);
        switch(opt) {
        case 'v':
            printf("received verbose flag -v\n");
            verbose_flg++;
            err_flg = 0;
            break;
        case 'f':
            printf("received file flag -f: %s\n", optarg);
            if (strlen(optarg) >= MAX_BUFFER_LEN)
            {
                // for simplicity. do not support dynamically allocate long string
                LOG("argument too long. exceeds maximum argument length. exiting gracefully...");
                exit(EXIT_FAILURE);
            }
            args.list = &argv[2];
            args.len = argc - 2;
            file_path = optarg;
            err_flg = 0;
            break;
        case 'a':
            printf("received argument flag -f: %s\n", optarg);
            // @TODO: parse arguments
            // args.list = &argv[2];
            // args.len = argc - 2;
            err_flg = 0;
            break;
        case 'h':
            printf("help menu\n");
            err_flg = 0;
            break;
        case ':':       /* -f or -o without operand */
            fprintf(stderr,
                "Option -%c requires an operand\n", optopt);
            err_flg = 1;
            break;
        case '?':
            fprintf(stderr,
                "Unrecognized option: '-%c'\n", optopt);
            err_flg = 1;
        }
    }
    if (err_flg) {
        fprintf(stderr, "usage: baby_vm -f <file-path> [-v] [-a] [-h (help)]");
        exit(2);
    }
    for ( ; optind < argc; optind++) {
        if (access(argv[optind], R_OK)) {
            printf("%s\n", argv[optind]);
        } else {
            printf("R not ok\n");
        }
    }

    if (verbose_flg) {
        LOG("after assigning new value to buffer");
        LOG(file_path);
        LOG("arg.list: ");
        LOG(*(args.list));
        LOG_COMP(MAX(argc-2, 0), args.len);
    }

    int16_t rv = process(&args, &file_path);

    // @TODO: need a better way to handle return value.
    // display return value
    printf("\n%d\n", rv);

    LOG("\nshutting down...\n");

    exit(EXIT_SUCCESS);
}
