//
// Created by agerberon on 18/10/23.
//

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>



#define MAX_PATH_LENGTH 4096
#define STDOUT 1
#define STDERR 2


#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] =
        {
                { "help",    no_argument,       0, 'h' },
                { "verbose", no_argument,       0, 'v' },
                { "input",   required_argument, 0, 'i' },
                { "output",  required_argument, 0, 'o' },
                { 0,         0,                 0,  0  }
        };

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char* binary_optstr = "hvi:o:";

void print_usage(char* bin_name) {
    fprintf(stdout, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}

/**
 * Procedure checks if a variable needs to be freed (checks: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated memory
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free) {
    if (to_free != NULL) free(to_free);
}

/**
 * Duplicate optarg as a string, handling errors
 * \see man 3 strndup
 * \see man 3 perror
 * \return a duplicated string or NULL in case of error
 */
char* dup_optarg_str() {
    char* str = NULL;

    if (optarg != NULL) {
        str = strndup(optarg, MAX_PATH_LENGTH);

        // Check if ERRNO is set
        if (str == NULL)
            perror(strerror(errno));
    }

    return str;
}

void print_file_metadata(const char* filename){
    struct stat file_stat;
    if(stat(filename, &file_stat) == -1){
        perror("");
        exit(1);
    }

    printf("%s - " , filename);
}
