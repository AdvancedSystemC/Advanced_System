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
            case 'v':
                //verbose mode
                is_verbose_mode = 1;
                break;
            case 'h':
                print_usage(argv[0]);

                free_if_needed(bin_input_param);

                exit(EXIT_SUCCESS);
            default :
                break;
        }
    }

    /**
     * Checking binary requirements
     * (could defined in a separate function)
     */
    if (bin_input_param == NULL)
    {
        dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

        // Freeing allocated data
        free_if_needed(bin_input_param);
        // Exiting with a failure ERROR CODE (== 1)
        exit(EXIT_FAILURE);
    }


    // Printing params
    dprintf(1, "** PARAMS **\n%-8s %s\n%-8s: %d\n",
            "input",   bin_input_param,
            "verbose", is_verbose_mode);

    // III - ls "like"

    // Get and format file permissions in the form
    void format_file_permissions(mode_t mode, char* permissions) {
        permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
        permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
        permissions[2] = (mode & S_IXUSR) ? 'x' : '-';
        permissions[3] = (mode & S_IRGRP) ? 'r' : '-';
        permissions[4] = (mode & S_IWGRP) ? 'w' : '-';
        permissions[5] = (mode & S_IXGRP) ? 'x' : '-';
        permissions[6] = (mode & S_IROTH) ? 'r' : '-';
        permissions[7] = (mode & S_IWOTH) ? 'w' : '-';
        permissions[8] = (mode & S_IXOTH) ? 'x' : '-';
        permissions[9] = '\0';  // Null-terminate the string
    }
    void print_file_information(const char* filename) {
        struct stat file_stat;
        if (stat(filename, &file_stat) == -1) {
            perror("");
            return;
        }

        // Extracting and formatting file permissions
        char permissions[10];
        format_file_permissions(file_stat.st_mode, permissions);

        // Extracting and printing other file information
        struct passwd* pw = getpwuid(file_stat.st_uid);
        struct group* gr = getgrgid(file_stat.st_gid);
        char timestamp[20];
        strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));

        printf("%s - Permissions: %s, Owner: %s, Group: %s, Size: %ld bytes, Modified: %s\n",
               filename, permissions, pw->pw_name, gr->gr_name, file_stat.st_size, timestamp);
    }

    //Open the directory given in arguments
    DIR* directory = opendir(bin_input_param);
    if (directory == NULL) {
        perror("");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry = NULL;

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) {
            // Print file metadata for regular files
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", bin_input_param, entry->d_name);
            print_file_information(file_path);
        }
    }

    closedir(directory);

    // Freeing allocated data
    free_if_needed(bin_input_param);

    return EXIT_SUCCESS;
}
