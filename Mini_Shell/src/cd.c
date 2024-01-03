#include <stdio.h>

int execute_cd(char **args)
{
    if (args[0] == NULL)
    {
        // No argument provided, change to home directory
        chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(args[0]) != 0)
        {
            perror("cd");
        }
    }
    return 1;
}
