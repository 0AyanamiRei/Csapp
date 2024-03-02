#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <getopt.h>

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

extern char **environ;

/* 声明函数 */
void unix_error(char *msg);
char *Fgets(char *ptr, int n, FILE *stream);
pid_t Fork(void);
void app_error(char *msg);





/* 实现函数 */

void app_error(char *msg) /* Application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

char *Fgets(char *ptr, int n, FILE *stream) 
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
	app_error("Fgets error");

    return rptr;
}

pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}


/* 函数解析 */


// 1
/* waitpid(-1, &status, 0)参数解释:
*   pid = -1: wait set 由所有子进程组成
*   &status:  将导致子进程返回的状态信息给status
*   options = 0 : 父进程被挂起,直到其wait set中的一个子进程终止
*   waitpid如果成功则返回子进程的PID, 其他错误返回-1或0
*  
*   解释status的宏
*   WIFEXITED(status): 子进程通过exit或者return正常终止, 返回true
*   WEXITSTATUS(status): 返回一个正常终止的子进程的退出状态 (exit(?)中的?)
*   
*   解释错误条件
*   wait set 为空集, 再调用waitpid就返回-1, 并设置errno为ECHILD
*   waitpid被signal中断, 返回-1, 并设置errno为EINTR
*   检查errno的值可以得知是否回收完当前进程的子进程
*/ 


// 2
/* int parseline(char *buf, char **argv):
*   解析cmdline的函数,返回值0或1,根据最后一个参数是否为'&'决定任务是否在后台运行
*   '&', return (bg = 1):background = 1
*   
*   解析的参数会传给argv这个二重指针
*/ 




