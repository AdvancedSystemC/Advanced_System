#ifndef TYPEDEF_H
#define TYPEDEF_H
/**
 * @struct Alias
 * @brief Structure representing a command-line alias
 */
struct Alias
{
    char *name;    // Alias name
    char *command; // Command assosiated with the alias
};

/**
 * @struct Command
 * @brief Struct to represent a command
 */
typedef struct
{
    char *executable;
    char **args;
    int arg_count;
    int has_pipe;
    int run_in_background;
    int has_input_redirection;
    char* input_file;
    int has_output_redirection;
    char* output_file;
    char *here_document_content;
    int append_output;
    struct Command *next_command;
} Command;

// Function signature for command execution
typedef int (*CommandFunction)(char **);

typedef struct
{
    const char *name;
    CommandFunction function;
} CommandTableEntry;

#endif
