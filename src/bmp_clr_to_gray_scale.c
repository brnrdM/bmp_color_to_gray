#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_PATH_N 4096

typedef struct opt_args {
    char *in;
    char *out;
} file_paths_t;

int parse_args(int argc, char **argv, file_paths_t *file_paths);
void print_usage(char *exec_name);

int
main (int argc, char *argv[])
{
    file_paths_t file_paths;
    
    if (parse_args(argc, argv, &file_paths)) {
        print_usage(argv[0]);
        free(file_paths.in);
        free(file_paths.out);
        exit(1);
    }

    if ((!file_paths.in) || (!file_paths.out)) {
        printf("Missing argument.\n");
        print_usage(argv[0]);
        free(file_paths.in);
        free(file_paths.out);
        exit(2);
    }

    printf("%s\n%s\n",file_paths.in, file_paths.out);

    return 0;
}

int
parse_args(int argc, char **argv, file_paths_t *file_paths)
{
    int c;
    opterr = 0;
    file_paths->in = NULL;
    file_paths->out = NULL;
    
    while ((c = getopt(argc, argv, "i:o:h")) != -1) {
        switch (c) {
            case 'i':
                file_paths->in = strndup(optarg, MAX_FILE_PATH_N);
                break;
            case 'o':
                file_paths->out = strndup(optarg, MAX_FILE_PATH_N);
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
