/* Read me!
 * 
 * 编写fgets函数的一个计时版本tfgets, 5秒后用户不输入信息则会返回NULL, 否则返回一个指向输入行的指针
 * 请注意,tfgets参数和fgest参数保持一致, 仅仅添加了计时功能.
*/
/*** Just try and believe yourself !! ***/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>



// char *fgets(char *s, int size, FILE *stream);

/* 实现的思路
    1. 如果我们能得到两个并行的进程1和进程2, 我们在进程1中进行fgets操作, 在进程2中进行sleep(time)
       在sleep(time)执行完毕后向进程1发送一个信号, 检查在进程1中是否有输入,这样就达到了计时的功能
    2. 实际上unistd.h库中已为我们实现了好了定时向调用进程(自身)发送信号的功能:
            unsigned int alarm(unsigned int seconds); 在seconds秒后发送信号SIGALRM

    询问过大模型后,我们得知当进程调用fgets等待用户输入的时候,如果被信号中断,fgets不会将stdin的数据写入到s
    而且会返回NULL
    查看网上的答案发现也是被中断后选择输出"NULL" 而不是保留已输入的内容

    整合copilot的回答,给出一个思路:
    我们每输入一个字符就保留下来, 然后不停的询问是否有信号传入, 以此来达到保留已输入内容的目的
    (疑惑,是否能接收到退格符然后做出对应的行为)
*/
#define time 5
typedef void (*sighandler_t)(int);
volatile char *res;

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

sighandler_t Signal(int signum, sighandler_t handler)
{
    if (signal(signum, handler) == SIG_ERR)
        unix_error("signal error");
}

void SIGALRM_handler(int sig)
{/* 我们应该在信号处理函数中选择返回 */
    char no_entry;
    if(no_entry){/* 在5s内没有输入则返回NULL */
        ;
    }
    else{ /* 在5s内有任何输入 则返回指针s */
        ;
    }

}

char *tfgets(char *s, int size, FILE *stream)
{
    Signal(SIGALRM, SIGALRM_handler);

    *res = fgets(s, size, stdin);
}
