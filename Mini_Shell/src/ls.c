#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

/**
 * Get and format file permissions in the form "rwxrwxrw-"
 */
void format_file_permissions(mode_t mode, char *permissions)
{
    permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[9] = '\0'; // Null-terminate the string
}

void print_file_metadata(const char *filename)
{
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1)
    {
        perror("");
        return;
    }

    // Extracting and formatting file permissions
    char permissions[10];
    format_file_permissions(file_stat.st_mode, permissions);

    // Extracting and printing other file information
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    char timestamp[20];
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));

    printf("%s - Permissions: %s, Owner: %s, Group: %s, Size: %ld bytes, Modified: %s\n",
           filename, permissions, pw->pw_name, gr->gr_name, file_stat.st_size, timestamp);
}

int execute_ls(char **toks)
{
    char *dir_to_open = ".";
    // open the directory given in argument
    if (toks[0] != NULL)
    {
        dir_to_open = toks[0];
    }

    DIR *directory = opendir(dir_to_open);
    if (directory == NULL)
    {
        perror("");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry = NULL;
    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_REG || entry->d_type == DT_DIR)
        {
            // Print file metadata for regular files
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", toks[1], entry->d_name);
            print_file_metadata(file_path);
        }
    }

    closedir(directory);

    return EXIT_SUCCESS;
}
