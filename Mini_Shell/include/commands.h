#ifndef COMMAND_H
#define COMMAND_H

// int execute_command();
int execute_exit(char **args);
int execute_ps(char **args);
int execute_cd(char **toks);
int execute_pwd(char **args);
int execute_echo(char **toks);
int execute_who(char **args);
int execute_history(char **args);

#endif // COMMAND_H
