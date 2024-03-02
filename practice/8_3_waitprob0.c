#include "/home/refrain/csapp/Csapp/code/chapter-8/ECF/mycsapp.c"

int main()
{
    if (fork() == 0) {
        printf("a");
        fflush(stdout);
    }
    else {
        printf("b");
        fflush(stdout);
        waitpid(-1, NULL, 0);
    }
    
    printf("c");
    fflush(stdout);
    exit(0);
}

/* 分析一下
    调用fork()函数:
    父进程中函数返回值等于子进程PID!=0所以执行else后的语句
    子进程中函数返回值等于0==0,所以满足条件执行if后的语句
    父进程由于调用waitpid函数被挂起,等待子进程被回收

    如果机器先执行父进程,那么输出结果是b|ac|c, b和c中间是子进程的输出
    如果机器先执行子进程,那么输出结果是ac|bc

    答案是acbc abcc bacc,绘制出进程图如下:


                  (·)-->printf("a")-->printf("c")-->exit(·)
                   ↑
    main(·)--→fork(·)-->printf("b")-->printf("c")-->exit(·)
    只要满足拓扑排序的规则即可，父进程b在c的前面，子进程a在c的前面,但是需要注意两个c
    我们这里用c1表示父进程输出的c，c2表示子进程输出的c，再来看一下答案：
    a c2 b c1
    a b c2 c1
    b a c2 c1
    由于父进程会被waitpid挂起，所以c2是一定在c1之前输出的
*/