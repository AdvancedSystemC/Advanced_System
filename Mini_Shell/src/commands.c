#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "../include/commands.h"
#include "../include/history.h"
#include "../include/alias.h"

/*
    //alias built-in command
    if(strcmp(toks[0],"alias") == 0){
        if(toks[1] != NULL && toks[2] != NULL){

            // Concatenate all tokens after the "=" character
            int command_length = 0;
            for (int i = 2; toks[i] != NULL; i++) {
                command_length += strlen(toks[i]);
            }

            char* command = malloc((command_length + 1) * sizeof(char));
            command[0] = '\0'; // Initialize the command string as an empty string

            for (int i = 2; toks[i] != NULL; i++) {
                strcat(command, toks[i]);
                if (toks[i + 1] != NULL) {
                    strcat(command, " "); // Add a space between tokens
                }
            }

            addAlias(toks[1],command);
        }
        else if(toks[1] == NULL && toks[2] == NULL){
            listAliases();
        }
        else if(toks[1] != NULL && toks[2] == NULL){
            printf(lookupAlias(toks[1]));
            printf("\n");
        }
        else{
            printf(toks[1]);
            printf("Error alias");
        }

    }

    if(strcmp(toks[0],"unalias") == 0){
        deleteAlias(toks[1]);
    }

    //alias-save built-in command
    if(strcmp(toks[0],"alias-save") == 0){
        SaveAliases();
    }

    //alias-load built-in command
    if(strcmp(toks[0],"alias-load") == 0){
        LoadAliases();
    }

    return 1;
}*/

int execute_exit(char **args)
{
    exit(0);
}

int execute_ps(char **args)
{
    DIR *proc_dir = opendir("/proc");

    if (!proc_dir)
    {
        perror("opendir");
        return;
    }

    printf("%-5s %-20s %s\n", "PID", "NAME", "STATUS");

    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
        {
            char status_path[256];
            snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

            FILE *status_file = fopen(status_path, "r");
            if (status_file)
            {
                char line[256];
                char name[256];
                char status[256];

                while (fgets(line, sizeof(line), status_file) != NULL)
                {
                    if (sscanf(line, "Name: %s", name) == 1)
                    {
                        break;
                    }
                }

                if (fgets(line, sizeof(line), status_file) != NULL)
                {
                    sscanf(line, "State: %s", status);
                }

                fclose(status_file);

                printf("%-5s %-20s %s\n", entry->d_name, name, status);
            }
        }
    }

    closedir(proc_dir);
}

int execute_cd(char **toks)
{
    if (toks[1] == NULL)
    {
        // No argument provided, change to home directory
        chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(toks[1]) != 0)
        {
            perror("cd");
        }
    }
    return 1;
}

int execute_pwd(char **args)
{
    char *cwd = (char *)malloc(sizeof(char) * 1024);
    if (getcwd(cwd, 1024) != NULL)
    {
        printf("%s\n", cwd);
        return 1;
    }
    else
    {
        perror("Cannot get current directory");
        return NULL;
    }
}

int execute_echo(char **toks)
{
    int i = 1;
    while (toks[i] != NULL)
    {
        if (toks[i][0] == '$')
        {
            char *env_var = getenv(toks[i] + 1);
            if (env_var != NULL)
            {
                printf("%s ", env_var);
            }
        }
        else
        {
            printf("%s ", toks[i]);
        }
        i++;
    }
    printf("\n");
    return 1;
}

int execute_who(char **args)
{
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        if (current_record.ut_type == USER_PROCESS)
        {
            printf("%-8.8s ", current_record.ut_user);
            printf("%-8.8s ", current_record.ut_line);

            time_t login_time = current_record.ut_tv.tv_sec;
            struct tm *lt = localtime(&login_time);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", lt);

            printf("%s ", time_str);
            printf("(%s)\n", current_record.ut_host);
        }
    }

    close(utmpfd);
    return 1;
}

int execute_history(char **args)
{
    printCommandHistory();
    return 1;
}