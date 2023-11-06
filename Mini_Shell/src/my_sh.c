#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 

// Greeting shell during startup 

typedef struct Node {
    char command[100];
    struct Node* prev;
    struct Node* next;
}CommandNode;

CommandNode* createNode(const char* command){
    CommandNode* newNode = (CommandNode*)malloc(sizeof(CommandNode));
    strcpy(newNode->command,command);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void addCommandToHistory(CommandNode** history, const char* command){
    CommandNode* newNode = createNode(command);

    if(*history == NULL){
        *history = newNode;
    }
    else{
        newNode->next=*history;
        (*history)->prev = newNode;
        *history = newNode;
    }
}

void printCommandHistory(CommandNode* history){
    int count = 1;
    while(history != NULL){
        printf("%d. %s\n", count, history->command);
        history = history->next;
        count++;
    }
}

void freeCommandHistory(CommandNode* history){
    while(history != NULL){
        CommandNode* temp = history;
        history = history-> next;
        free(temp);
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
    CommandNode* history = NULL;

    //Test historique
    addCommandToHistory(&history,"ls");
    addCommandToHistory(&history,"cd ..");
    addCommandToHistory(&history,"touch historique");
    printCommandHistory(history);
    
    char* username = getenv("USER");
    char input[1024];
    init_shell();

    while (1){
        printf("%s:%s$",username,getDirectory());
        fgets(input, sizeof(input), stdin);
        printf("\n");
    }

    return 0;
}