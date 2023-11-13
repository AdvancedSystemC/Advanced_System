/**
 * @file alias.h
 * @brief This file contains function declarations for alias-related operations
*/

#ifndef ALIAS_H
#define ALIAS_H

/**
 * @brief Initializes the alias manager
*/
void initAliasManager();

/**
 * @brief Add a new alias to the manager
 * @param name The name of the aliases
 * @param command The command associated with the alias
 * @return 1 if the alias is added succesfully otherwise 0
*/
int addAlias(const char* name , const char* command);

/**
 * @brief Delete a new alias to the manager
 * @param name The name of the aliases
 * @param command The command associated with the alias
 * @return 1 if the alias is removed succesfully otherwise 0
*/
int deleteAlias( const char* name);

/**
 * @brief Lists all defined aliases
*/
void listAliases();

/**
 * @brief Looks up the command associated with the given alias
 * @param name The name of the alias to be looked up
 * @return The command associated with the alias or "Alias not found" if the alias is not found
*/
const char* lookupAlias(const char* name);

/**
 * @brief Load aliases from a file for persistence
 * @return 1 if aliases are loaded successfully , 0 otherwise
*/
int LoadAliases();

/**
 * @brief Save aliases to a file for persistence
 * @return 1 if aliases are saved successfully , 0 otherwise
*/
int SaveAliases();

/**
 * @brief Cleans up ressources used by the alias manager. 
 */
void cleanupAliasManager();

#endif // ALIAS_H