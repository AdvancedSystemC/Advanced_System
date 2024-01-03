#include <stdio.h>

int execute_echo(char **args)
{
    printf("Executing built-in echo command\n");

    int i = 0;
    while (args[i] != NULL)
    {
        printf("%s ", args[i]);
        i++;
    }
    
    while (args[i] != NULL)
    {
        if (args[i][0] == '$')
        {
            char *env_var = getenv(args[i] + 1);
            if (env_var != NULL)
            {
                printf("%s ", env_var);
            }
        }
        else
        {
            printf("On passe dans le if\n");
            printf("%s\n ", args[i]);
        }
        i++;
    }
    return 1;
}