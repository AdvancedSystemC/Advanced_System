#include <stdio.h>

int execute_pwd(char **args)
{
    char *cwd = (char *)malloc(sizeof(char) * 1024);
    if (getcwd(cwd, 1024) != NULL)
    {
        printf("%s\n", cwd);
        return 1;
    }
    else
    {
        perror("Cannot get current directory");
        return NULL;
    }
}