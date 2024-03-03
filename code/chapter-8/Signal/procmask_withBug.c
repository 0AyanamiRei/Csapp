/* WARNING: This code is buggy! */

#include"/home/refrain/csapp/Csapp/code/mycsapp.c"
/*解释各个函数:
    Sio_error(char s[]); 打印字符串s并且终止进程;
    Execve(const char *filename, char *const argv[], char *const envp[]) execve(filename, argv, envp) < 0 则终止进程
    SIGCHLD:默认行为=忽略(进程忽略掉该信号), 事件=子进程停止或者终止
*/

/* 为什么有race condition?
        child立马exit-------+
        |         1         ↓
····---fork()---addjob()---(handler)deletejob()
        |_______2______|
如果从路线1的时间比路线2更小,那么就会先进行deletejob再进行addjob
*/

void handler(int sig)
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0)
    { /* Reap a zombie child */
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(pid); /* Delete the child from the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if (errno != ECHILD) /* errno == EINTR同理, 回收被signal打断 */
        Sio_error("waitpid error");
    errno = olderrno;
}

int main(int argc, char **argv)
{
    int pid;
    sigset_t mask_all, prev_all;

    Sigfillset(&mask_all);
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1)
    {
        if ((pid = Fork()) == 0)
        { /* Child process */
            Execve("/bin/date", argv, NULL);
        }
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); /* Parent process */
        addjob(pid);                                  /* Add the child to the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    exit(0);
}