# My shell

由于之前写的bug太多了,太狗屎了,懒得改了,所以我决定在吸取前面的教训后不再继续第八章的homework而是直接开始写shlab.

## 包库问题

由于在我的电脑```wsl2 Ubuntu20.04```上C程序中的```signal.h```并不支持```sigsetjmp```和```siglongjmp```,但是我在写的时候又会用到
所以我统一使用了```C++```, 同时也可以避免再写一个链表的麻烦,直接使用```C++```的```STL```即可.

## 进程与进程组结构体的设计

阅读教材```OSTEP```后, 我看到了有关```xv6```中的```proc```结构体的设计,同时书上也提醒了我们内核应该为每个进程保存信息以便追踪这些进程,所以我决定
专门思考一下该如何设计这个结构体,以用尽量少的内存传递更多信息, 下面是我的实现.

```c
struct proc{
    int pid;
    int pgid;
    int status;

    char *proc_args[];
}
```

- ```pid```与```pgid```分别记录该进程的进程ID和进程组ID
- ```status```记录进程处于何种状态, ```stopped=0```和```running=1```
- ```*proc_args[]```记录进程运行任务的参数,以便在被回收的时候输出检查
  
## 基础框架

shell的基本框架基于homework中对前台的处理, 即```当前台任务运行时候shell被挂起```,这里可以直接借鉴书上讲解```sigsuspend```函数时的代码

```c
volatile sig_atomic_t pid;

void sigchld_handler(int s)
{
    int olderrno = errno;
    pid = Waitpid(-1, NULL, 0);
    errno = olderrno;
}

int main(int argc, char **argv)
{
    sigset_t mask, prev;
    Signal(SIGCHLD, sigchld_handler);
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    while (1) {
        Sigprocmask(SIG_BLOCK, &mask, &prev);

        if (Fork() == 0) {/* do something in child process */};
        pid = 0;
        while (!pid) sigsuspend(&prev);/* wait for the fg job accomplished */

        Sigprocmask(SIG_SETMASK, &prev, NULL);
        printf(".");
        {/* now the shell(parent process) can do something*/}
    }
    exit(0);
}
```

在这个基础上, 我们解析传入的命令行,以及加上前后台进程的判断:

```c
/* ······一些准备工作 */
while (1) {
    printf("Hello, Kurusi >");
    fgets(cmdline, MAXLINE, stdin);
    if (feof(stdin)) exit(0);

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    if (!builtin_command(argv)){
        sigprocmask(SIG_BLOCK, &mask, &prev);

        if ((tmp_pid = fork()) == 0) { /* do something in child process */
            if (execve(argv[0], argv, environ) < 0) {
                sigprocmask(SIG_SETMASK, &prev, NULL);
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            } else { /* parent process you can do something here */}
        }
    }

    if(!bg){
        tmp_pid = 0;
        while (tmp_pid != fg_pid) sigsuspend(&prev);/* wait for the fg job accomplished */
        
        {/* now the shell(parent process) can do something*/}

    }else{
        {/* now the shell(parent process) can do something*/}
    }

    sigprocmask(SIG_SETMASK, &prev, NULL);
}
```

先从前台进程入手,对于一个前台作业shell必须要等待它结束或者是I/O端的信号终止它, 一旦前台进程结束了,我们必须要回收它,然后等待下一条指令的输入
对于shell挂起这个逻辑的实现```while (tmp_pid != fg_pid) sigsuspend(&prev);```, 是因为我们在```SIGCHLD```的信号处理函数中回收子进程的时候
如果成功回收会将```tmp_pid```赋值为回收的进程ID, 以此来表示前台任务结束(注意, ```sigsuspend```函数原子的暂时接触对```SIGCHLD```的屏蔽)

## 后台进程管理

## 信号处理函数

完成该实验我们需要的处理的信号:

- ```SIGCHLD(进程结束)```
- ```SIGINT(Ctrl+C)```
- ```SIGTSTP(Ctrl+Z)```
