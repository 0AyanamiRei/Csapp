#include "/home/refrain/csapp/Csapp/code/mycsapp.c"
/* Question:
    
    Tell me what will input this program? 
    it's 213 but not 212, let me tell you why:
    简单来说,父进程将信号SIGUSR1传给子进程后,子进程通过handler1这个信号处理程序捕获并且处理信号
    在handler1中输出--counter, 这个counter是子进程自己的变量所以输出1

    对于父进程中的输出,在父进程中,输出了counter和++counter,这是属于父进程的变量,所以输出是2和3
    由于在输出3之前调用了Waitpid(-1, NULL, 0), 父进程被挂起,等待子进程处理信号输出1后再输出3
    所以顺序是2-1-3
*/


volatile long counter = 2;

void handler1(int sig)
{
    sigset_t mask, prev_mask;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask); /* Block sigs */
    Sio_putl(--counter); /* 打印long类型变量 1 */
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    _exit(0);
}

int main()
{
    pid_t pid;
    sigset_t mask, prev_mask;

    printf("%ld", counter); /* input 2 */
    fflush(stdout);

    /* SIGUSR1: 默认行为=终止, 用户自定义的信号1*/
    signal(SIGUSR1, handler1);
    if ((pid = Fork()) == 0) /* Child process */
    {
        while (1)
        {
        };
    }
    Kill(pid, SIGUSR1); /* Father process will send the signal to child process */
    Waitpid(-1, NULL, 0);

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask); /* Block sigs */
    printf("%ld", ++counter); /* input 3*/
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    exit(0);
}

/* output 213 */