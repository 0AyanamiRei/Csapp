#include "mycsapp.c"

#define MAXARGS 128


/* 函数声明 */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);


int main(int argc, char *argv[], char *envp[])
{
    char cmdline[MAXLINE]; /* Command line */

    while (1) { 
        printf("Ciallo~(∠・ω< )⌒☆  > ");
        Fgets(cmdline, MAXLINE, stdin); /* Read */
        if (feof(stdin)){ /* 检查末尾的字符是否是Ctrl+D */
            exit(0);
        }

        /* Evaluate */
        eval(cmdline);
    }
}

/* eval - Evaluate a command line */
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE]; /* Holds modified command line */
    int bg; /* Background (bg) or Forwardground (fg) */
    pid_t pid; /* Process id */

    strcpy(buf, cmdline); /* buf = cmdline */
    bg = parseline(buf, argv); /* 解析命令行 */

    if (argv[0] == NULL)
        return; /* Ignore empty lines */

    if (!builtin_command(argv)) {
        if ((pid = Fork()) == 0) { /* Child runs user job */
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
    }

    if (!bg) { /* Parent waits for foreground job to terminate */
        int status;
        if (waitpid(pid, &status, 0) < 0)
            unix_error("waitfg: waitpid error");
    }
    else
        printf("%d %s", pid, cmdline);
        /* BUG!!  没有回收bg的任务就退出了 */
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
    /* 在这里实现用户输入的指令 */
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "&")) /* Ignore singleton & */
        return 1;
    return 0;
}

/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv)
{
    char *delim; /* Points to first space delimiter */
    int argc; /* Number of args */
    int bg; /* Background job tag */

    /* Preprocess string of the cmdline */
    buf[strlen(buf)-1] = ' '; /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    buf++;

    argv[argc] = NULL;

    if (argc == 0) /* Ignore blank line */
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}