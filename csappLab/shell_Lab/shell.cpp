/*** Just try and believe yourself !! ***/
#include "shell.h"
#include "func.cpp"

int main(int argc, char **argv)
{
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;

    sigset_t mask, prev;
    signal(SIGCHLD, SIGCHLD_handler);
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    while (1) {
        printf("Hello, Kurusi >");
        fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)) exit(0);

        strcpy(buf, cmdline);
        bg = parseline(buf, argv);

        if (!builtin_command(argv)){
            sigprocmask(SIG_BLOCK, &mask, &prev);

            if ((tmp_pid = fork()) == 0) { /* do something in child process */
                if (execve(argv[0], argv, environ) < 0) {
                    sigprocmask(SIG_SETMASK, &prev, NULL);
                    printf("%s: Command not found.\n", argv[0]);
                    exit(0);
                } else { /* parent process you can do something here */}
            }
        }

        if(!bg){
            tmp_pid = 0;
            while (tmp_pid != fg_pid) sigsuspend(&prev);/* wait for the fg job accomplished */
            
            {/* now the shell(parent process) can do something*/}

        }else{
            {/* now the shell(parent process) can do something*/}
        }

        sigprocmask(SIG_SETMASK, &prev, NULL);
    }
    exit(0);
}