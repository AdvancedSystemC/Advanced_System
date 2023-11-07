#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <dirent.h>

#include "../include/history.h"
#include "../include/display.h"
#include "../include/commands.h"
#include "../include/input.h"

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

void free_tokens(char* toks[],int count){
    for(int i = 0; i < count; i++){
        free(toks[i]);
    }
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
            read_input(input, sizeof(input));

            addCommandToHistory(input);

            char* toks[1024];
            process_input(input, delimiter, toks);

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