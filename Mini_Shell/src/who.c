#include <utmp.h>
#include <fcntl.h>

int execute_who(char **args)
{
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        if (current_record.ut_type == USER_PROCESS)
        {
            printf("%-8.8s ", current_record.ut_user);
            printf("%-8.8s ", current_record.ut_line);

            time_t login_time = current_record.ut_tv.tv_sec;
            struct tm *lt = localtime(&login_time);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", lt);

            printf("%s ", time_str);
            printf("(%s)\n", current_record.ut_host);
        }
    }

    close(utmpfd);
    return 1;
}