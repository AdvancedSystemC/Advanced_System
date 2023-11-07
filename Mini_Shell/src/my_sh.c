#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 

FILE* historyFile;

void green() {
    printf("\033[0;32m");
}

void blue() {
    printf("\033[0;34m");
}

void reset () {
  printf("\033[0m");
}

void addCommandToHistory(const char* command) {
    historyFile = fopen("history.txt", "a+"); // Open history file in append mode
    fprintf(historyFile, "%s\n", command);
    fclose(historyFile);
}

void printCommandHistory() {
    char command[100];
    int count = 1;
    rewind(historyFile); // Move file pointer to the beginning
    while (fgets(command, sizeof(command), historyFile) != NULL) {
        printf("%d. %s", count, command);
        count++;
    }
}

void init_shell() 
{ 
    system("clear");
    printf("******************"
        "************************"); 
    printf("\n\n\n\t****  MY_SH  ****"); 
    printf("\n\n\tCreated by : DUVERNAY Benoit feat GERBERON Alexandre"); 
    printf("\n\n\n\n*******************"
        "***********************"); 
    printf("\n\n\n\n"); 
    sleep(1); 
    //clear(); 
}

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

int main()
{
    char* username = getenv("USER");
    char input[1024];
    init_shell();

    //Test historique
    addCommandToHistory("ls");
    addCommandToHistory("cd ..");
    addCommandToHistory("touch historique");
    printCommandHistory();

    while (1){
        green();
        printf("%s",username);
        reset();
        printf(":");
        blue();
        printf("%s",getDirectory());
        reset();
        printf("$ ");
        fgets(input, sizeof(input), stdin);
        addCommandToHistory(input);
    }

    return 0;
}