/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include "common.c"



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

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);
    free_if_needed(buffer);


    return EXIT_SUCCESS;
}
