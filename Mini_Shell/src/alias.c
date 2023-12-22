#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/alias.h"
#include "../include/typedef.h"

#define MAX_ALIASES 100

struct Alias *aliasList[MAX_ALIASES];
FILE *alias = "alias.txt";
int aliasCount = 0;

void initAliasManager()
{

    for (int i = 0; i < MAX_ALIASES; i++)
    {
        aliasList[i] = NULL;
    }

    aliasCount = 0;

    LoadAliases();
}

int addAlias(const char *name, const char *command)
{

    // Check if the alias already exists
    for (int i = 0; i < aliasCount; i++)
    {
        if (strcmp(aliasList[i]->name, name) == 0)
        {
            printf("Alias '%s' already exists \n", name);
        }

        return 0;
    }

    // Check if the alias count exceeds the maximum limit
    if (aliasCount >= MAX_ALIASES)
    {
        printf("Maximum number of aliases reached , cannot add alias \n");
        return 0;
    }

    // Allocate memory for the new alias
    struct Alias *newAlias = (struct Alias *)malloc(sizeof(struct Alias));
    if (newAlias == NULL)
    {
        printf("Memory allocation error. Cannot add alias");
        return 0;
    }

    // Copy the name and the command of the new alias
    newAlias->name = strdup(name);
    newAlias->command = strdup(command);

    aliasList[aliasCount] = newAlias;
    aliasCount++;

    printf("Alias '%s' added\n", name);

    return 1;
}

int deleteAlias(const char *name)
{
    int deleted = 0;

    for (int i = 0; i < aliasCount; i++)
    {
        if (strcmp(aliasList[i]->name, name) == 0)
        {
            free(aliasList[i]->name);
            free(aliasList[i]->command);
            free(aliasList[i]);

            // Fill the gap in the array
            for (int j = i; j < aliasCount - 1; j++)
            {
                aliasList[j] = aliasList[j + 1];
            }

            aliasList[aliasCount - 1] = NULL;
            aliasCount--;

            printf("Alias '%s' deleted \n", name);

            deleted = 1;
            break;
        }
    }

    if (!deleted)
    {
        printf("Alias '%s' not found , Nothing Deleted \n", name);
    }

    return deleted;
}

void listAliases()
{
    if (aliasCount == 0)
    {
        printf("No aliases defined \n");
    }
    else
    {
        printf("List of aliases: \n");
        for (int i = 0; i < aliasCount; i++)
        {
            printf("%s = %s\n", aliasList[i]->name, aliasList[i]->command);
        }
    }
}

const char *lookupAlias(const char *name)
{
    for (int i = 0; i < aliasCount; i++)
    {
        if (strcmp(aliasList[i]->name, name) == 0)
        {
            return aliasList[i]->command;
        }
    }

    return "Alias not found"; // Alias not found
}

int LoadAliases()
{
    FILE *file = fopen(alias, "r");
    if (file == NULL)
    {
        perror("Error opening file for reading aliases");
        return 0;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *name = strtok(buffer, "=");
        char *command = strtok(NULL, "\n");

        if (name != NULL && command != NULL)
        {
            addAlias(name, command);
        }
    }

    fclose(file);
    return 1;
}

int SaveAliases()
{
    FILE *file = fopen(alias, "w");
    if (file == NULL)
    {
        perror("Error opening file for writing aliases");
        return;
    }

    for (int i = 0; i < aliasCount; i++)
    {
        fprintf(file, "%s=%s\n", aliasList[i]->name, aliasList[i]->command);
    }

    fclose(file);
    return 1;
}

void cleanupAliasManager()
{
    for (int i = 0; i < aliasCount; i++)
    {
        free(aliasList[i]->name);
        free(aliasList[i]->command);
        free(aliasList[i]);
    }

    aliasCount = 0;
}