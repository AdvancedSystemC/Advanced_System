/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<getopt.h>

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
void print_usage(char* bin_name)
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
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

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


/**
 * 
*/
void print_file_metadata(const char* filename){
  struct stat file_stat;
  if(stat(filename, &file_stat) == -1){
    perror("");
    exit(1);
  }

  printf("%s - " , filename);
} 



/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;
  char* bin_input_param = NULL;
  char* bin_output_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'i':
        //input param
        if (optarg)
        {
          bin_input_param = dup_optarg_str();         
        }
        break;
      case 'o':
        //output param
        if (optarg)
        {
          bin_output_param = dup_optarg_str();
        }
        break;
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
 
        exit(EXIT_SUCCESS);
      default :
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
          "input",   bin_input_param, 
          "output",  bin_output_param, 
          "verbose", is_verbose_mode);

  // I - Copie de Fichier
  int original;
  int copy;

  // In order to copy the content of one file to another , you first need to open both
  // Opening first file in read only
  original= open(bin_input_param, O_RDONLY);
  if(original == -1){
    fprintf(STDERR ,"Error in opening the file, %s" , bin_input_param);
    exit(1);
  }

  // Opening the second file ( create one if it does not exist) , with Read and Write permission
  copy = open(bin_output_param,O_WRONLY|O_CREAT,0755);
  if(copy == -1){
    //We choose stderr as output stream for the error
    fprintf(STDERR , "Error in opening the file, %s" , bin_output_param);
    exit(1);
  }

  /*printf("original file descriptor: %d\n", original);
  printf("copy file descriptor: %d\n", copy);*/

  int bytesRead;
  int bytesWrote;

  char* buffer = malloc(sizeof(char)*4096);

  //Read the first block of 4096 bytes
  bytesRead = read(original,buffer,4096);

  //the loop stops when the block of bytes = 0
  while(bytesRead > 0){
    bytesWrote = write (copy,buffer, bytesRead);
      
    if(bytesWrote != bytesRead){
      perror("Error while copying the file!");
      exit(1);
    }

    //read the next block of bytes
    bytesRead = read(original,buffer,4096);

    // read function returns 0 when it reaches the EOF
    if(bytesRead == 0){
      printf("File copied successfully!\n");
    }
  }

  // II - Print << reverse >>
  int file;
  int n;

  file = open(bin_input_param, O_RDONLY);
  if(file == -1){
    fprintf(STDERR ,"Error in opening the file, %s" , bin_input_param);
    exit(1);
  }

  off_t offset = lseek(file, 0, SEEK_END);
  //char* buffer = malloc(sizeof(char)*4096);

  for(int i = offset -1; i >= 0 ; i--){
    lseek(file, (off_t)i , SEEK_SET);
    n = read(file,buffer,1);
    fprintf(STDOUT , "%c",buffer[0]);
  }

  close(file);


  // III - ls "like"

  //Open the directory given in arguments
  DIR* directory = opendir(bin_input_param);
  if(directory == NULL){
    perror("");
  }

  struct dirent* entry = NULL;

  while((entry = readdir(directory)) != NULL){
    if(entry->d_type == DT_REG){
      print_file_metadata(entry->d_name);
    }
  }

  closedir(directory);

  // Freeing allocated data
  free_if_needed(bin_input_param);
  free_if_needed(bin_output_param);
  free_if_needed(buffer);


  return EXIT_SUCCESS;
}
