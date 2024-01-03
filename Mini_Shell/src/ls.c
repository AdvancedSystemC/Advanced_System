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

// Structure pour stocker les informations sur les fichiers
struct FileData {
    char file_path[MAX_PATH_LENGTH];
    off_t file_size;
};

// Fonction de comparaison pour qsort()
int compare_file_size(const void* a, const void* b) {
    const struct FileData* file_a = (const struct FileData*)a;
    const struct FileData* file_b = (const struct FileData*)b;

    return (file_a->file_size - file_b->file_size);
}

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

int execute_ls(char **args)
{
    int recursive = 0;
    int list = 0;
    int show_all = 0;
    int show_inode = 0;
    int no_options = 1;

    // Parsing options
    if (args[0] != NULL)
    {
        if (strcmp(args[0], "-R") == 0)
        {
            recursive = 1;
            no_options = 0;
        }
        else if (strcmp(args[0], "-l") == 0)
        {
            list = 1;
            no_options = 0;
        }
        else if (strcmp(args[0], "-a") == 0)
        {
            show_all = 1;
            no_options = 0;
        }
        else if (strcmp(args[0], "-i") == 0)
        {
            show_inode = 1;
            no_options = 0;
        }
        else
        {
            no_options = 1;
        }
    }
    char *dir_to_open = ".";
    // open the directory given in argument
    if (no_options == 0 && args[1] != NULL)
    {
        dir_to_open = args[1];
    }
    else if (no_options == 1 && args[0] != NULL)
    {
        dir_to_open = args[0];
    }
    // Open the directory given in arguments
    DIR *directory = opendir(dir_to_open);

    if (directory == NULL)
    {
        perror("");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry = NULL;

    while ((entry = readdir(directory)) != NULL)
    {
        if (show_all == 1)
        {
            printf("%s\n", "show all");
        }
        if (show_all == 0 && entry->d_name[0] == '.')
        {
            continue; // Ignorer les fichiers cachés sauf si -a est spécifié
        }

        if (recursive && (entry->d_type == DT_REG || entry->d_type == DT_DIR))
        {
            return execute_ls_recursive(dir_to_open);
        }

        if (list && (entry->d_type == DT_REG || entry->d_type == DT_DIR))
        {
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", dir_to_open, entry->d_name);
            print_file_metadata(file_path);
        }
        if (show_inode && (entry->d_type == DT_REG || entry->d_type == DT_DIR))
        {
            printf("%ld ", entry->d_ino);
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", dir_to_open, entry->d_name);
            printf("%s ", file_path);
            // Maintenant, vous pouvez utiliser file_path pour afficher ou manipuler le chemin avec l'inode
        }
        if ((no_options || show_all) && (entry->d_type == DT_REG || entry->d_type == DT_DIR))
        {
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", dir_to_open, entry->d_name);
            // print file_path
            printf("%s\n", file_path);
        }
    }

    closedir(directory);

    return EXIT_SUCCESS;
}
int execute_ls_recursive(const char *dir_to_open)
{
    // Ouvrir le répertoire donné en argument
    DIR *directory = opendir(dir_to_open);
    if (directory == NULL)
    {
        perror("");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry = NULL;

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            // Afficher les répertoires seulement si ce ne sont pas "." ou ".."
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", dir_to_open, entry->d_name);
            printf("%s\n", file_path);

            // Récursion pour parcourir les sous-répertoires
            execute_ls_recursive(file_path);
        }
        else if (entry->d_type == DT_REG)
        {
            // Afficher les fichiers réguliers
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", dir_to_open, entry->d_name);
            printf("%s\n", file_path);
        }
    }

    closedir(directory);

    return EXIT_SUCCESS;
}