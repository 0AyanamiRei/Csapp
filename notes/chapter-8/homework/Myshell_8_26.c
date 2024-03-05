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
#define LEN 64

typedef struct Job
{
    pid_t pid;
    pid_t pgid;
    int argc;
    char state; /*stopped or running*/
    char **argv;
    Job *next;   /*point to next job*/
}Job;

/* global 变量*/
Job* bg_head = NULL;
Job *fg_job;
Job *tmp_jobs;
pid_t ppid;


/* job's linklist function */
void add_job(Job* bg_head, Job* job);
void delete_job(Job* bg_head, pid_t id);
Job* search_job(Job* bg_head, pid_t id);


typedef void (*sighandler_t)(int);
volatile char *res;
extern **environ;

/* pre function */
void unix_error(char *msg);
pid_t Fork(void);
sighandler_t Signal(int signum, sighandler_t handler);
void Free(void* ptr);



/* main function */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
int jobs(char **argv);
int bg(char **argv);
int fg(char **argv);



/* signal handler function*/
void SIGCHLD_handler(int sig);
void SIGINT_handler(int sig);
void SIGTSTP_handler(int sig);
void SIGUSR1_handler(int sig);


/* test function */
void foo(char **argv);

void pf_wait(int bg){
    if(!bg) printf("Now, we are waitting for the fg job accomplished.\n");
}




/* 设计思路
    使用parseline()函数解析命令行的内容, 返回值表示前后台任务, 并设置argv为输入name和参数

  解决第一个问题, 如何设计才能让shell在"前台作业完成","Ctrl+Z"和"Ctrl+C"这三个情况任意发生一个之前被挂起?
    对于shell主进程, 我们选择在接收一个指令并fork一个前台作业后, 调用sigsuspend挂起, 这样在三个情况任意发生一个之前
    shell都会处于挂起状态, 并且会在接收到信号后立刻接触.
    ***但是问题进而演变了-> 怎么区分前台作业结束和后台作业结束?
    对于Ctrl+Z和Ctrl+C我们能保证信号来源确切是我们期望的, 但是对于SIGCHLD信号是来自前台作业还是后台作业, 并不知道,
    思考后我们发现, SIGCHLD信号被设置为SIG_IGN后,父进程会忽略掉这个信号,从而不会解除挂起状态, 如果能够在前台作业完成
    后给父进程发送一个SIGUSR信号,而后台作业不发生,那么就可以识别前后台作业了, 但是前台作业完成后怎么才能发送SIGUSR呢?
    这个时候就必须要全局的数据结构去存放这种信息了,这个问题我们留给设计数据结构,也就是问题二中继续讨论.


  解决第二个问题, 如何设计数据结构去管理后台的作业才恰当?
    我打算实现一个链表去动态的管理后台作业, 根据copilot的建议,如果能够将一个作业的信息传递给信号处理函数,那么
    我们就能区分前台与后台SIGCHLD的问题了. 查阅书上关于waitpid的内容 "如果wait set中的一个进程在刚调用的时刻就已经终止了
    那么waitpid立即返回终止的子进程的pid", 根据pid就可以在全局的数据结构中找到其进程的所有信息,所以我们在SIGCHLD的
    处理函数中使用wait set既做到回收已终止子进程又可以获取终止子进程的信息.
    ***当我们获取到来自前后台作业结束而发出的SIGCHLD信号后, 我们可以用kill(pid_t pid, int sig)向父进程发送信号来解除
    挂起的状态, 因此我们要获取父进程的PID, 同样使用全局变量去存储这个信息

  解决第三个问题, 对于Ctrl+Z和Ctrl+C信号按kernel->shell->fg的顺序发送有什么简便的地方?
    对于我们使用的shell, 上述两个信号会由内核直接发送给fg_job, 当子进程发生结束,停止,继续的行为时都会向父进程发送
    SIGCHLD信号,这为SIGCHLD信号处理函数带来了很大的不便.

*/


int main(int argc, char **argv, char **envp) {
    bg_head = (Job*)malloc(sizeof(Job));
    char cmdline[MAXLINE]; /* Command line */
    ppid = getppid();

    do{ 
        printf("Ciallo~(∠・ω< )⌒☆ > ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)){
            exit(0);
        }
        eval(cmdline);
    } while(TRUE);

    exit(0);
}



/* pre function */

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

void Free(void *ptr){
    free(ptr);
    ptr = NULL;
}

/* main function */

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

    /* builtin_command return 0 表明可能是一个可执行程序 */
    if (!builtin_command(argv)) {
        if ((pid = Fork()) == 0) {
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
    }

    if (!bg) {
        /* 等待: 1,fg job完成 2,SIGINT信号 3,SIGTSTP信号 */
        pf_wait(bg);
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGCHLD); /* 通过信号处理函数让识别前台job还是后台job */
        sigsuspend(mask); 
    }
    else
        pf_wait(bg);
        printf("%d %s", pid, cmdline);
    }
    return;
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

int builtin_command(char **argv)
{
    if(!strcmp(argv[0], "quit"))
    {
        exit(0);/* 结束shell进程 */
    } else if(!strcmp(argv[0], "&"))
    {
        return 1; /* 无效的 */
    } else if(!strcmp(argv[0], "jobs"))
    {
        jobs(argv);
    } else if(!strcmp(argv[0], "bg"))
    {

    } else if(!strcmp(argv[0], "fg"))
    {

    } else if(!strcmp(argv[0], "foo"))
    {
        foo(argv);
    } else
    { /* 这是一个可执行程序 */
        return 0;
    }
}

int jobs(char **argv){
    ;
}

int bg(char **argv){

}

int fg(char **argv){
    ;
}


/* signal handler function*/

void SIGCHLD_handler(int sig){
    char check_bg = TRUE; /* 依赖全局数据结构的信息得到前后台进程 */
    int status = 0;
    /* 收到该信号意味着有子进程终止了,由父进程调用处理函数 */
    pid_t pid = waitpid(-1, &status, WNOHANG);

    if(pid == fg_job->pid)
        check_bg = FALSE;

    if(!check_bg) { /* 前台作业 */
        if(WIFEXITED(status)){ /* 正常退出(注意没有编写exit或return的子进程默认exit(0)) */
            printf("fg child %d terminated by status %d\n", pid, status);
        }
        else if (WIFSIGNALED(status)) {
            char buf[LEN];
            sprintf(buf, "fg child %d terminated by signal %d", pid, WTERMSIG(status));
            psignal(WTERMSIG(status), buf);
        }else {
            printf("fg child %d terminated abnormally\n", pid);
        }
        Free(fg_job); /* 最好是对前台作业清空状态 */
        kill(ppid, SIGUSR1); /* 发送信号给shell,解除sigsuspend状态 */
    }
    else { /* 后台作业 */
        delete_job(bg_head, pid);
        if(WIFEXITED(status)){
            printf("bg child %d terminated by status %d\n", pid, status);
        }
        else if (WIFSIGNALED(status)) {
            char buf[LEN];
            sprintf(buf, "bg child %d terminated by signal %d", pid, WTERMSIG(status));
            psignal(WTERMSIG(status), buf);
        }else {
            printf("bg child %d terminated abnormally\n", pid);
        }
    }
}

void SIGUSR1_handler(int sig){/* 这个信号仅仅提醒shell前台作业完成, 什么都不需要做 */
    ;
}

void SIGINT_handler(int sig){
    ;
}

void SIGTSTP_handler(int sig){
    ;
}


/* job's linklist function */
void add_job(Job* bg_head, Job* job){
    if(bg_head->next == NULL) {
        bg_head->next = job;
        return;
    }
    job->next = bg_head->next;
    bg_head->next = job;
}

void delete_job(Job* bg_head, pid_t id){
    Job* cur = bg_head;
    while (cur->next != NULL) {
        if (cur->next->pid == id) {
            Job* to_delete = cur->next;
            cur->next = to_delete->next;
            free(to_delete);
            return;
        }
        cur = cur->next;
    }
}

Job* search_job(Job* bg_head, pid_t id) {
    Job* current = bg_head->next;
    while (current != NULL) {
        if (current->pid == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}



/* test function */
void foo(char **argv){
    sleep(5);
    exit(123);
}