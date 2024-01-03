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

    // II - Print << reverse >>
    int file;
    int n;

    file = open(bin_input_param, O_RDONLY);
    if(file == -1){
        fprintf(STDERR ,"Error in opening the file, %s" , bin_input_param);
        exit(1);
    }

    off_t offset = lseek(file, 0, SEEK_END);
    char* buffer = malloc(sizeof(char)*4096);

    for(int i = offset -1; i >= 0 ; i--){
        lseek(file, (off_t)i , SEEK_SET);
        n = read(file,buffer,1);
        printf("%c",buffer[0]);
    }

    close(file);

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);
    free_if_needed(buffer);


    return EXIT_SUCCESS;
}
