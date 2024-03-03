/* It's usefule but inefficient !!  cycle is a waste !!*/

#include"/home/refrain/csapp/Csapp/code/mycsapp.c"

/* 设计的初衷
    当Linux shell创建一个fg-job(前台作业)的时候, 我们必须等待其终止才能继续接受命令.
    也就是说在这个fg-job发出SIGCHLD信号被SIGCHLD信号处理程序回收之前父进程都必须挂起.
    
    第一代处理手段: while (!pid); 等待信号处理程序回收子进程
Wasteful!!!
main···--→fork()--→pid=0--→while(continue check······)--->printf()···
            ↓              |_______wasteful_________|↑
    child:  +---···-→exit()-->handler:pid=xxx--------+
***************************************************************************************
    第二代处理手段: while (!pid) pause(); 等待信号中断pause();
BUG!!!
race condition:
在调用pause()之前子进程就已经exit,内核就将SIGCHLD信号发送给父进程,那么父进程会被pause一直挂起
main···--→fork()--→pid=0--→while(check)------→pause()
            ↓                             ↑ 
    child:  +---···-→exit()               ↑
                kernel send SIGCHLD-------+
***************************************************************************************
    第三代处理手段: while (!pid) sleep(1); 等待时间太久了,程序会很慢
***************************************************************************************
    第四代处理手段: while (!pid) sigsuspend(&prev);
函数原型：int sigsuspend(const sigset_t *mask);
等价于原子版(atomic version):
    {
        sigprocmask(SIG_BLOCK, &mask, &prev);
        pause();
        sigprocmask(SIG_SETMASK, &prev, NULL);
    }

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
            sigsuspend(&prev); /* now parent can get the SIGCHLD and stop this function*/

        /* Do some work after receiving SIGCHLD */
        printf(".");
    }
    exit(0);
}