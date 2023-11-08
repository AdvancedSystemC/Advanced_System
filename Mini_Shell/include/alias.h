#ifndef ALIAS_H
#define ALIAS_H

void initAliasManager();
int addAlias(const char* name , const char* command);
int deleteAlias( const char* name);
void listAliases();
const char* lookupAlias(const char* name);
int LoadAliases();
int SaveAliases();
void cleanupAliasManager();

#endif // ALIAS_H