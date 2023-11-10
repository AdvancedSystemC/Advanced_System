#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <dirent.h>

#include "../include/history.h"
#include "../include/display.h"
#include "../include/commands.h"
#include "../include/input.h"
#include "../include/typedef.h"
#include "../include/alias.h"

/**
 * @brief Get the current working directory.
 * @return The current working directory as a string.
 */
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

/**
 * @brief Free the memory allocated for an array of tokens.
 * @param toks The array of tokens.
 * @param count The number of tokens in the array.
 */
void free_tokens(char* toks[],int count){
    for(int i = 0; i < count; i++){
        free(toks[i]);
    }
}


/**
 * @brief The main function of the shell program.
 * @return 0 on successful execution.
 */
int main()
{
    initAliasManager();

    if(isatty(STDIN_FILENO)){
        //shell is running in interactive mode
        char* username = getenv("USER");
        char hostname[1024];
        gethostname(hostname,sizeof(hostname));
        char input[1024];
        const char* delimiter = " \t\n="; // Define the token delimiter

        display_welcome();

        while (1){

            display_prompt(username,hostname,getDirectory());
            read_input(input, sizeof(input));

            addCommandToHistory(input);

            char* toks[1024];
            process_input(input, delimiter, toks);

            //TO DO
            // Separate input into command / options / path

            //Execute command here
            execute_command(toks);

            //TO DO Fix free tokens
            //free_tokens(toks,sizeof(toks)/sizeof(toks[0]));
        }
    }
    else{
        //shell is running in non-interactive mode
    }

    return 0;
}