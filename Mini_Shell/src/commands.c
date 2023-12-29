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