#include "func_shell.cpp"

void debug() {
    printf("3\n");
    sleep(1);
    printf("2\n");
    sleep(1);
    printf("1\n");
    sleep(1);
}

int main()
{
    sigset_t mask, prev;

    Signal(SIGCHLD, SIGCHLD_handler);

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    /*************add**************/
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    int bg;
    int argc;
    initjobs();
    /*************add**************/
    while (1)
    {
        /*************add**************/
        print_String("Ciallo~(∠・ω< )⌒☆ > ");
        Fgets(cmdline, MAXLINE, stdin);
        bg = parseline(cmdline, argc, argv);
        /*************add**************/
        sigprocmask(SIG_BLOCK, &mask, &prev); /* 在父进程中屏蔽掉SIGCHLD信号 */
        if(!builtin_command(argv)){
            if ((tmp_pid = Fork()) == 0){ /* child */
                sigprocmask(SIG_SETMASK, &prev, NULL); /* 在子进程中解除SIGCHLD的屏蔽 */
                execve(argv[0], argv, environ); /* 重新加载代码,也就是说后面的代码只跟父进程有关 */
            }
            else { /* parent */
                Job newJob = {tmp_pid, Running, argv};
                if(!bg) fg_pid = tmp_pid;
                else {
                    addjobs(newJob);
                }
            }
        }

        if(!bg){ /* 前台作业 */
            tmp_pid = 0;
            while (tmp_pid != fg_pid)
                sigsuspend(&prev); /*在sigsuspend调用之前都要屏蔽掉SIGCHLD信号避免race condition*/


            sigprocmask(SIG_SETMASK, &prev, NULL);
        }else { /* 后台作业 */

        
        ;
        }
        
        
        /* you can do something */
        debug();
    }
    exit(0);
}