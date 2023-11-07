#include <stdio.h>
#include "display.h"
#include "colors.h"

void display_welcome() 
{ 
    system("clear");
    printf("******************"
        "************************"); 
    printf("\n\n\n\t****  MY_SH  ****"); 
    printf("\n\n\tCreated by : DUVERNAY Benoit feat GERBERON Alexandre"); 
    printf("\n\n\n\n*******************"
        "***********************"); 
    printf("\n\n\n\n"); 
}

void display_prompt(const char* username ,const char* hostname , const char* directory){
    green();
    printf("%s@%s",username,hostname);
    reset();
    printf(":");
    blue();
    printf("%s",directory);
    reset();
    printf("$ ");
}