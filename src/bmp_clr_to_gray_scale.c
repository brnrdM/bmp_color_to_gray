#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/lib_bmp.h"

#define MAX_FILE_PATH_N 4095

typedef struct opt_args {
    char in[MAX_FILE_PATH_N + 1];
    char out[MAX_FILE_PATH_N + 1];
} file_paths_t;

int parse_args(int argc, char **argv, file_paths_t *file_paths);
void print_usage(char *exec_name);

int
main (int argc, char *argv[])
{
    file_paths_t file_paths;
    memset(&file_paths, 0, sizeof(file_paths_t));

    if (parse_args(argc, argv, &file_paths)) {
        print_usage(argv[0]);
        exit(1);
    }

    if ((!file_paths.in[0]) || (!file_paths.out[0])) {
        printf("Missing argument.\n");
        print_usage(argv[0]);
        exit(2);
    }

    FILE *fp;

    DEBUG_PRINT("IN:%s\nOUT:%s\n",file_paths.in, file_paths.out);

    fp = fopen(file_paths.in, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open %s\n", file_paths.in);
        exit(2);
    }

    bmp_header_t bmp;
    int err = bmp_header_init(&bmp, fp);
    if (err) {
        fprintf(stderr, "%s is not a valid bmp file\n", file_paths.in);
        exit(2);
    }
    if (DEBUG) {
        bmp_print_header(&bmp);
    }

    FILE *fout;

    fout = fopen(file_paths.out, "wb");
    if (!fout) {
        fprintf(stderr, "Failed to open %s\n", file_paths.out);
        exit(2);
    }

    bmp_convert_to_gs(&bmp, fp, fout);

    fclose(fout);
    fclose(fp);

    return 0;
}

int
parse_args(int argc, char **argv, file_paths_t *file_paths)
{
    int c;
    opterr = 0;

    while ((c = getopt(argc, argv, "i:o:h")) != -1) {
        switch (c) {
            case 'i':
                strncpy(file_paths->in, optarg, MAX_FILE_PATH_N);
                break;
            case 'o':
                strncpy(file_paths->out, optarg, MAX_FILE_PATH_N);
                break;
            case 'h':
                return 1;
            default:
                printf("Invalid option: %c\n", optopt);
                return 1;
        }
    }

    return 0;
}

void
print_usage(char *exec_name)
{
    printf("Usage: %s [OPTIONS]\n", exec_name);
    printf(" -i         Input File Path\n");
    printf(" -o         Output File Path\n");
    printf(" -h         Help\n");
}
