/* It's usefule but inefficient !!  cycle is a waste !!*/

#include"/home/refrain/csapp/Csapp/code/mycsapp.c"

/* 设计的初衷
    当Linux shell创建一个fg-job(前台作业)的时候, 我们必须等待其终止才能继续接受命令.
    也就是说在这个fg-job发出SIGCHLD信号被SIGCHLD信号处理程序回收之前父进程都必须挂起.
*/

volatile sig_atomic_t pid;

void sigchld_handler(int s)
{
    int olderrno = errno;
    pid = waitpid(-1, NULL, 0);
    errno = olderrno;
}

void sigint_handler(int s)
{
}

int main(int argc, char **argv)
{
    sigset_t mask, prev;

    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    while (1)
    {
        Sigprocmask(SIG_BLOCK, &mask, &prev); /* Block SIGCHLD */
        if (Fork() == 0)                      /* Child */
            exit(0);

        /* Parent */
        pid = 0;
        Sigprocmask(SIG_SETMASK, &prev, NULL); /* Unblock SIGCHLD */

        /* Wait for SIGCHLD to be received (wasteful) */
        while (!pid)
            ;

        /* Do some work after receiving SIGCHLD */
        printf(".");
    }
    exit(0);
}