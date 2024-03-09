#include "shell.h"


int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit"))
        exit(0);
    if (!strcmp(argv[0], "&"))
        return 1;
    return 0;
}

int parseline(char *buf, char **argv)
{
    char *delim;
    int argc;
    int bg;
    buf[strlen(buf) - 1] = ' ';
    while (*buf && (*buf == ' '))
        buf++;
    argc = 0;
    while ((delim = strchr(buf, ' ')))
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))
            buf++;
    }
    argv[argc] = NULL;
    if (argc == 0)
        return 1;
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;
    return bg;
}




void SIGCHLD_handler(int s)
{
    int olderrno = errno;
    tmp_pid = waitpid(-1, NULL, 0);
    errno = olderrno;
}