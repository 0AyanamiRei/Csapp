#include "shell.h"

                /****************************************/
                /************pre function****************/
                /****************************************/
void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    _exit(0);
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

char *Fgets(char *ptr, int n, FILE *stream)
{
char *rptr;

if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
    unix_error("Fgets error");

return rptr;
}

void print_int(int value) {
    char buffer[20];  // 足够大以容纳任何整数
    int i = 0;

    // 特殊情况：value为0
    if (value == 0) {
        write(STDOUT_FILENO, "0", 1);
        return;
    }

    // 将整数转换为字符串
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    // 反转字符串
    for (int j = 0; j < i / 2; ++j) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    // 输出字符串
    write(STDOUT_FILENO, buffer, i);
}

void print_String(char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}


                    /****************************************/
                    /************main function***************/
                    /****************************************/
void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    int argc;
    sigset_t mask, prev;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    strcpy(buf, cmdline);
    bg = parseline(buf, argc, argv);


    if (!builtin_command(argv))
    {
        if ((fg_pid = Fork()) == 0) { /* child */
            execve(argv[0], argv, environ);
        }
    }
    if (!bg) {
        fg_pid = 0;
        while (!fg_pid) {
            sigsuspend(&prev);
        }  
    } 
}

int parseline(char *buf, int& argc, char **argv)
{
    char *delim;
    int bg;

    buf[strlen(buf) - 1] = ' ';
    while (*buf && (*buf == ' '))
        buf++;

    argc = 0;
    while ((delim = strchr(buf, ' ')))
    {
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

    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}

int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit"))
    {
        _exit(0);
    }
    else if (!strcmp(argv[0], "&"))
    {
        return 1;
    }
    else if (!strcmp(argv[0], "jobs"))
    {
        jobs(argv);
    }
    else if (!strcmp(argv[0], "bg"))
    {
        bg(argv);
    }
    else if (!strcmp(argv[0], "fg"))
    {
        fg(argv);
    }
    else
    { /* 这是一个可执行程序 */
        return 0;
    }
}

int jobs(char **argv)
{
    print_String("We will show all of the jobs:····\nAll right,to be refining····\n");
}

int bg(char **argv)
{
    print_String("We will restart the job to bg:PID=%d, ····\nAll right,to be refining····\n");
}

int fg(char **argv)
{
    print_String("We will restart the job to fg:PID=%d, ····\nAll right,to be refining····\n");
}



                /****************************************/
                /*******signal handler function**********/
                /****************************************/
void SIGCHLD_handler(int sig)
{
    int olderrno = errno;
    print_String("接收到SIGCHLD信号!\n");
    tmp_pid = waitpid(-1, NULL, 0);
    errno = olderrno;
}

void SIGUSR1_handler(int sig)
{ /* 这个信号仅仅提醒shell前台作业完成, 什么都不需要做 */
    print_String("接收到SIGUSR1信号!\n");
}

void SIGINT_handler(int sig)
{/* Ctrl+C 来自键盘的中断信号 */
    print_String("接收到SIGINT信号!\n");
}

void SIGTSTP_handler(int sig)
{/* Ctrl+Z 来自键盘的中断信号 */
    print_String("接收到SIGTSTP信号!\n");
}



                /****************************************/
                /*******jobs' data struct**********/
                /****************************************/
void initjobs(){
    printf("初始化作业表\n");
}
void addjobs(Job newJob){
    printf("添加作业pid = %d 到作业表\n", tmp_pid);
    Jobs.push_back(newJob);
}