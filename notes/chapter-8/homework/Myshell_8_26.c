/* Read me!
 * 希望你的shell有以下特性
 * 对于用户的指令, 由一个"name"+0/N个参数组成, 由一个或多个空格隔开
 * ->对于内置的命令(name=ls、jobs···)我们希望shell立刻处理它,并且等待下一条指令
 * ->否则假设其是一个可执行文件,在一个子进程的上下文加载并运行它
 * JOB的ID和子进程的ID用%来区别 %5表示jobID 5表示PID
 * 指令结束加上&表示在后台运行,否则前台
 * 
 * 
 * 希望你的shell支持以下指令:
 * 1 输入Ctrl+C(+Z)后信号按 kernel-> shell -> fg 转送;     (试试能不能跳过中间层)
 * 2 内置命令jobs列出所有后台作业;
 * 3 bg <ID>发送一个信号给作业或进程,重启它,并在后台运行它;  (注意<ID>既可能表示作业也可以表示进程)
 * 4 fg <ID>发送一个信号给作业或进程,重启它,并在前台运行它;
 * 5 shell能够回收所有zombie 进程, 如果任何作业因为收到未捕获信号而终止, 输出一条信息到终端!
*/

/*** Just try and believe yourself !! ***/

/* question: 如果我们bg一个前台作业的ID会怎么样? */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <asm-generic/signal.h>



#define MAXARGS 128
#define MAXLINE 128
#define TRUE 1
#define FALSE 0

typedef struct job
{
    pid_t pid;
    pid_t pgid;
    int argc;
    char state; /*stopped or running*/
    char **argv;
    job *pre;   /*point to next job*/
}job;

/* 添加一个job到bg */


typedef void (*sighandler_t)(int);
volatile char *res;
extern **environ;

/* pre function */
void unix_error(char *msg);
pid_t Fork(void);
sighandler_t Signal(int signum, sighandler_t handler);



/* main function */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);



/* 设计思路
    使用parseline()函数解析命令行的内容, 返回值表示前后台任务, 并设置argv为输入name和参数

  解决第一个问题, 如何设计才能让shell在"前台作业完成","Ctrl+Z"和"Ctrl+C"这三个情况任意发生一个之前被挂起?
    对于shell主进程, 我们选择在接收一个指令并fork一个前台作业后, 调用sigsuspend挂起, 这样在三个情况任意发生一个之前
    shell都会处于挂起状态, 并且会在接收到信号后立刻接触.
    ***但是问题进而演变了-> 怎么区分前台作业结束和后台作业结束?
    对于Ctrl+Z和Ctrl+C我们能保证信号来源确切是我们期望的, 但是对于SIGCHILD信号是来自前台作业还是后台作业, 并不知道,
    思考后我们发现, SIGCHILD信号被设置为SIG_IGN后,父进程会忽略掉这个信号,从而不会解除挂起状态, 如果能够在前台作业完成
    后给父进程发送一个SIGUSR信号,而后台作业不发生,那么就可以识别前后台作业了, 但是前台作业完成后怎么才能发送SIGUSR呢?

  解决第二个问题, 如何设计数据结构去管理后台的作业才恰当?
    我打算实现一个链表去动态的管理后台作业, 根据copilot的建议,如果能够将一个作业的信息传递给信号处理函数,那么
    我们就能区分前台与后台SIGCHILD的问题了.





*/


int main(int argc, char **argv, char **envp) {
    char cmdline[MAXLINE]; /* Command line */

    
    do{ 
        printf("Ciallo~(∠・ω< )⌒☆ > ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)){
            exit(0);
        }
        eval(cmdline);

        /* 等待: 1,fg job完成 2,SIGINT信号 3,SIGTSTP信号 */
        sigset_t mask;
        sigfillset(&mask);
        sigsuspend(mask); 

    } while(TRUE);

    exit(0);
}









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

void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    if (argv[0] == NULL)
        return;

    if (!builtin_command(argv)) {
        if ((pid = Fork()) == 0) {
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
    }

    if (!bg) {
        int status;
        if (waitpid(pid, &status, 0) < 0)
            unix_error("waitfg: waitpid error");
    }
    else
        printf("%d %s", pid, cmdline);
    }
    return;
}

int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit"))
        exit(0);
    if (!strcmp(argv[0], "&"))
        return 1;
    return 0;
}

int parseline(char *buf, char **argv)
{
    char *delim;
    int argc;
    int bg;

    buf[strlen(buf)-1] = ' ';
    while (*buf && (*buf == ' '))
        buf++;

    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))
            buf++;
    }
    buf++;

    argv[argc] = NULL;

    if (argc == 0)
        return 1;

    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}