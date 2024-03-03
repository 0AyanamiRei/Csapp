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

#define time 5

// char *fgets(char *s, int size, FILE *stream);

/* 实现的思路
    1. 如果我们能得到两个并行的进程1和进程2, 我们在进程1中进行fgets操作, 在进程2中进行sleep(time)
       在sleep(time)执行完毕后向进程1发送一个信号, 检查在进程1中是否有输入,这样就达到了计时的功能
    2. 实际上unistd.h库中已为我们实现了好了定时向调用进程(自身)发送信号的功能:
            unsigned int alarm(unsigned int seconds); 在seconds秒后发送信号SIGALRM


*/


char *tfgets(char *s, int size, FILE *stream)
{
    char *res = fgets(s, size, stream);
}
