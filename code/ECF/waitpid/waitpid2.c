/****************************************************
改进后,程序会按照父进程创建子进程的顺序回收子进程 

这里提供了两份改进的代码，第一份main1是我自己写的()，第二份是书上给的
在我的机器上,第一份代码的输出是：
show the all PID: 
the 0th PID is 5672; the 1th PID is 5673; the 2th PID is 5674; 
now it's the 0th child process: PID = 5672, terminated normally, exit status=100
now it's the 0th child process: PID = 5673, terminated normally, exit status=101
now it's the 0th child process: PID = 5674, terminated normally, exit status=102
show the all PID: 
the 0th PID is -1; the 1th PID is 5673; the 2th PID is 5674; 
show the all PID: 
the 0th PID is -1; the 1th PID is -1; the 2th PID is 5674; 
show the all PID: 
the 0th PID is -1; the 1th PID is -1; the 2th PID is -1;
仔细读一下这个代码和输出结果会发现, 在i=0的第一个for循环中,子进程就会全部回收，
i保持不变的情况下为什么输出的pid=pid[i]=pid[0]还是正确的？
这很简单，只是我在阅读的时候忽略掉了一个问题 while ((pid[i] = waitpid(-1, &status, 0)) > 0)
在执行pid[i] = waitpid(-1, &status, 0)时, 父进程会挂起等待wait set中的任一子集被回收，回收成功的时候
会返回其PID给pid[i], 也就是说，在这里我们反复读写pid[i]的值,所以才造成了i不变，pid[i]在变化的情况
刚好按照fork时的顺序回收只是在这台机器上的凑巧，并不能消除这份不确定性。

我们应该使用waitpid(pid[i], &status, 0)这样的回收子进程方式如main.2所示，这样可以保证在任何机器上
回收的顺序一定是按照fork创建时的顺序
****************************************************/ 

#include "/home/refrain/csappLab/code/ECF/mycsapp.c"
#define N 3

void showPID(pid_t pid[]) {
    printf("show the all PID: \n");
    for(int j =  0; j < N; ++j) {
        printf("the %dth PID is %d; ", j, pid[j]);
    }
    printf("\n");
}


void main1()
{
    int status, i;
    pid_t pid[N];

    /* 为当期进程创建N个子进程 */
    for (i = 0; i < N; i++)
        if ((pid[i] = Fork()) == 0)
            exit(100 + i);

    for(i = 0; i < N; ++i){
        showPID(pid);
        /* 关于waitpid查看mycsapp.c中函数解析1 */
        while ((pid[i] = waitpid(-1, &status, 0)) > 0)
        {
            printf("now it's the %dth child process: ", i);
            if (WIFEXITED(status))
                printf("PID = %d, terminated normally, exit status=%d\n", pid[i], WEXITSTATUS(status));
            else
                printf("PID = %d, terminated abnormally\n", pid[i]);
        }
    }
    
    showPID(pid);
    if (errno != ECHILD)
        unix_error("waitpid error");
    exit(0);
}

void main2()
{
    int status, i;
    pid_t pid[N], retpid;

    for (i = 0; i < N; i++)
        if ((pid[i] = Fork()) == 0)
            exit(100 + i);

    i = 0;
    while ((retpid = waitpid(pid[i++], &status, 0)) > 0)
    {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n",
                   retpid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", retpid);
    }

    if (errno != ECHILD)
        unix_error("waitpid error");

    exit(0);
}


int main()
{
    main1();
}