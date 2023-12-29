#include <stdio.h>

int execute_cd(char **toks)
{
    if (toks[1] == NULL)
    {
        // No argument provided, change to home directory
        chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(toks[1]) != 0)
        {
            perror("cd");
        }
    }
    return 1;
}
