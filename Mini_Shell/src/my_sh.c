#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <dirent.h>

#include "history.h"
#include "display.h"

char* getDirectory(){
    char* cwd = (char*)malloc(sizeof(char) * 1024);
    if(getcwd(cwd,1024) != NULL){
        return cwd;
    }
    else{
        perror("Cannot get current directory");
        return NULL;
    }
}


void parse_input(const char* input , char* toks[] , const char* delimiter){
    int counter = 0;
    char* token = strtok((char*)input,delimiter);

    while (token != NULL) {
        toks[counter] = strdup(token);
        token = strtok(NULL, delimiter);
        counter++;
    }

    toks[counter] = NULL; // Terminate the toks array with a NULL
}

void free_tokens(char* toks[],int count){
    for(int i = 0; i < count; i++){
        free(toks[i]);
    }
}

void list_processes() {
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

int execute_command(char* toks[]){
    if(toks[0] == NULL){
        return 0;
    }

    //Exit built-in command
    if(strcmp(toks[0],"exit") == 0){
        exit(0);
    }

    //Ps built-in command
    if(strcmp(toks[0],"ps") == 0){
        list_processes();
    }

    //History built-in command
    if(strcmp(toks[0],"history") == 0){
        printCommandHistory();
    }

    return 1;
}


int main()
{
    if(isatty(STDIN_FILENO)){
        //shell is running in interactive mode
        char* username = getenv("USER");
        char hostname[1024];
        gethostname(hostname,sizeof(hostname));
        char input[1024];
        const char* delimiter = " \t\n"; // Define the token delimiter

        display_welcome();

        while (1){

            display_prompt(username,hostname,getDirectory());
            fgets(input, sizeof(input), stdin);

            // Remove the newline character from the input
            input[strcspn(input,"\n")]=0;

            addCommandToHistory(input);

            char* toks[1024];
            parse_input(input , toks , delimiter);

            //Execute command here
            execute_command(toks);

            //free_tokens(toks,sizeof(toks)/sizeof(toks[0]));
        }
    }
    else{
        //shell is running in non-interactive mode
    }

    return 0;
}