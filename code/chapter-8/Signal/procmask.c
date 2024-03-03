/* Good job! you avoid the race condition!*/
#include"/home/refrain/csapp/Csapp/code/mycsapp.c"

/*race condition:
        child立马exit-------+
        |         1         ↓
····---fork()---addjob()---(handler)deletejob()
        |_______2______|
如果从路线1的时间比路线2更小,那么就会先进行deletejob再进行addjob
*/

/* avoid the race condition:
    究其根本race的情况是：父进程在addjob(子进程)之前,子进程就已经将信号SIGCHLD传给了父进程并交给
    handler处理,导致父进程被挂起,先进行deletejob()无效操作, 所以我们只需要让addjob()这个操作和fork()
    操作稳定在一起执行即可, 那么在fork()到addjob()过程之间屏蔽掉SIGCHLD信号即可
*/

/* 有关信号屏蔽的函数解析见mycsapp.c 函数解析4 */

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
    sigset_t mask_all, prev_CHLD, mask_CHLD;

    Sigfillset(&mask_all);
    Sigemptyset(&mask_CHLD);
    Sigaddset(&mask_CHLD, SIGCHLD);
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1)
    {
        Sigprocmask(SIG_BLOCK, &mask_CHLD, &prev_CHLD); /* Block the SIGCHLD for parent */
        if ((pid = Fork()) == 0)
        { /* Child process */
            Sigprocmask(SIG_SETMASK, &prev_CHLD, NULL); /* Unblock the SIGCHLD for child */
            Execve("/bin/date", argv, NULL);
        }
         /* Parent process */
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(pid);                                  /* Add the child to the job list */
        Sigprocmask(SIG_SETMASK, &prev_CHLD, NULL);
    }
    exit(0);
}