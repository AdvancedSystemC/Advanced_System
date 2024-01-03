
int execute_exit(char **args)
{
    // Print the args array
    for (int i = 0; i < 2; i++) {
        printf("args[%d]: %s\n", i, args[i]);
    }
    
    exit(0);
}