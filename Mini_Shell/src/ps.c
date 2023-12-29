#include <stdio.h>
#include <dirent.h>

int execute_ps(char **args)
{
    DIR *proc_dir = opendir("/proc");

    if (!proc_dir)
    {
        perror("opendir");
        return;
    }

    printf("%-5s %-20s %s\n", "PID", "NAME", "STATUS");

    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
        {
            char status_path[256];
            snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

            FILE *status_file = fopen(status_path, "r");
            if (status_file)
            {
                char line[256];
                char name[256];
                char status[256];

                while (fgets(line, sizeof(line), status_file) != NULL)
                {
                    if (sscanf(line, "Name: %s", name) == 1)
                    {
                        break;
                    }
                }

                if (fgets(line, sizeof(line), status_file) != NULL)
                {
                    sscanf(line, "State: %s", status);
                }

                fclose(status_file);

                printf("%-5s %-20s %s\n", entry->d_name, name, status);
            }
        }
    }

    closedir(proc_dir);
}