#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "../include/commands.h"
#include "../include/history.h" // Include the necessary headers for command functionality.

int execute_command(char* toks[]){
    if(toks[0] == NULL){
        return 0;
    }

    //Exit built-in command
    if(strcmp(toks[0],"exit") == 0){
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
    // Logic for the "history" command.
    printCommandHistory(); // Assuming you have a function for printing command history.
    return 1; // Indicate that the shell should continue.
}