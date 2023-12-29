#include <stdio.h>

int execute_echo(char **toks)
{
    int i = 1;
    while (toks[i] != NULL)
    {
        if (toks[i][0] == '$')
        {
            char *env_var = getenv(toks[i] + 1);
            if (env_var != NULL)
            {
                printf("%s ", env_var);
            }
        }
        else
        {
            printf("%s ", toks[i]);
        }
        i++;
    }
    printf("\n");
    return 1;
}