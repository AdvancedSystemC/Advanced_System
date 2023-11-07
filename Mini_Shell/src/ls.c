#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

void ls(const char *dir, int op_a, int op_l){
    //https://iq.opengenus.org/ls-command-in-c/
    //https://medium.com/@winfrednginakilonzo/guide-to-code-a-simple-shell-in-c-bd4a3a4c41cd

    struct dirent *entry;
    DIR* directory = opendir(dir);

    if(!directory){
        if(errno = ENOENT){
            perror("Directory doesn't exist");
        }
        else{
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }

    //While the next entry is not readable we will print directory files
	while ((entry = readdir(directory)) != NULL)
	{
		//If hidden files are found we continue
		if (!op_a && entry->d_name[0] == '.')
			continue;
		printf("%s  ", entry->d_name);
		if(op_l) printf("\n");
	}
	if(!op_l)
	printf("\n");
}