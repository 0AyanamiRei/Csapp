#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

pid_t getpid(void); //获取调用者的PID
pid_t getppid(void);//获取调用者父进程的PID


void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork(void) /* 为了避免大量的错误检测使代码臃肿所以用Fork代替fork+错误检查 */
{
    pid_t pid;  
    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}