#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "../include/commands.h"
#include "../include/history.h" 
#include "../include/alias.h"

int execute_command(char* toks[]){
    if(toks[0] == NULL){
        return 0;
    }

    //Exit built-in command
    if(strcmp(toks[0],"exit") == 0){
        cleanupAliasManager();
        execute_exit();
    }

    //Ps built-in command
    if(strcmp(toks[0],"ps") == 0){
        execute_ps();
    }

    //History built-in command
    if(strcmp(toks[0],"history") == 0){
        execute_history();
    }

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
}

int execute_exit() {
    exit(0);
}

int execute_ps() {
    DIR* proc_dir = opendir("/proc");

    if (!proc_dir) {
        perror("opendir");
        return;
    }

    printf("%-5s %-20s %s\n", "PID", "NAME", "STATUS");

    struct dirent* entry;
    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            char status_path[256];
            snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

            FILE* status_file = fopen(status_path, "r");
            if (status_file) {
                char line[256];
                char name[256];
                char status[256];

                while (fgets(line, sizeof(line), status_file) != NULL) {
                    if (sscanf(line, "Name: %s", name) == 1) {
                        break;
                    }
                }

                if (fgets(line, sizeof(line), status_file) != NULL) {
                    sscanf(line, "State: %s", status);
                }

                fclose(status_file);

                printf("%-5s %-20s %s\n", entry->d_name, name, status);
            }
        }
    }

    closedir(proc_dir);
}

int execute_history() {
    printCommandHistory();
    return 1;
}