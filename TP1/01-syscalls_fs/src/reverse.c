/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
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

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

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
void print_usage(char *bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}

/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void *to_free)
{
  if (to_free != NULL)
    free(to_free);
}

/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char *dup_optarg_str()
{
  char *str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);

    // Checking if ERRNO is set
    if (str == NULL)
      perror(strerror(errno));
  }

  return str;
}

/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] =
    {
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char *binary_optstr = "hvi:o:";

/**
 *
 */
void print_file_metadata(const char *filename)
{
  struct stat file_stat;
  if (stat(filename, &file_stat) == -1)
  {
    perror("");
    exit(1);
  }

  printf("%s - ", filename);
}

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char **argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;
  char *bin_input_param = NULL;
  char *bin_output_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
    case 'i':
      // input param
      if (optarg)
      {
        bin_input_param = dup_optarg_str();
      }
      break;
    case 'o':
      // output param
      if (optarg)
      {
        bin_output_param = dup_optarg_str();
      }
      break;
    case 'v':
      // verbose mode
      is_verbose_mode = 1;
      break;
    case 'h':
      print_usage(argv[0]);

      free_if_needed(bin_input_param);
      free_if_needed(bin_output_param);

      exit(EXIT_SUCCESS);
    default:
      break;
    }
  }

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  if (bin_input_param == NULL || bin_output_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);
    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
  }

  // Printing params
  dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n",
          "input", bin_input_param,
          "output", bin_output_param,
          "verbose", is_verbose_mode);

  // II - Print << reverse >>
  int file;
  int n;

  file = open(bin_input_param, O_RDONLY);
  if (file == -1)
  {
    fprintf(stderr, "Error in opening the file, %s", bin_input_param);
    exit(1);
  }

  off_t offset = lseek(file, 0, SEEK_END);
  if (offset == -1)
  {
    perror("Error seeking to the end of the file");
    exit(1);
  }

  char *buffer = malloc(sizeof(char) * 4096);
  if (buffer == NULL)
  {
    fprintf(stderr, "Error allocating memory for buffer");
    exit(1);
  }

  for (int i = offset - 1; i >= 0; i--)
  {
    if (lseek(file, (off_t)i, SEEK_SET) == -1)
    {
      perror("Error seeking in the file");
      exit(1);
    }
    n = read(file, buffer, 1);
    if (n == -1)
    {
      perror("Error reading from the file");
      exit(1);
    }
    fprintf(stdout, "%c", buffer[0]);
  }

  close(file);

  // Freeing allocated data
   free_if_needed(bin_input_param);
   free_if_needed(bin_output_param);
   free_if_needed(buffer);

  return EXIT_SUCCESS;
}
