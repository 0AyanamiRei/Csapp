/* task：
    write a program named snooze with a single command-line argument, use the argument call
    the function snooze from 8_5_snooze.c. and terminate the function by input Ctrl+C from
    the keyboard such as:
    > ./snooze 5
    (Ctrl+C)
    Slept for 3 of 5 secs.
    > ···
*/

/* Mission Accomplished：
    以防第一遍读不明白到底要让我们干什么，我建议你应该实际运行一下调用snooze函数后按Ctrl+C会发生什么。

    并没有得到sleep剩下的时间，而是直接退出了程序，查阅Linux信号, 我们发现Ctrl+C对应的信号是SIGINT k=2
    是直接终止程序, 所以我们需要利用signal函数修改SIGINT的默认行为
*/


#include "/home/refrain/csapp/Csapp/code/chapter-8/ECF/mycsapp.c"

/* 顺带学习一下: 函数指针 */
typedef void (*sighandler_t)(int);

void sigint_handler(int sig);
unsigned int snooze(unsigned int secs);

/* 顺带学习一下: 函数指针 */
sighandler_t handler = sigint_handler;

int main(int argc, char *argv[], char *envp[])
{/* Just try!! */
    signal(SIGINT, handler);

    int time = atoi(argv[1]);
    printf("Now your PC will sleep about %ds, you can enter Ctrl+C to wake up it!\n", time);
    snooze(time);

    /* If we take it here, can it be usefule? */
    /* signal(SIGINT, handler); */
    /* Answer is : No! */

    printf("Now time is going\n");
    exit(0);
}

void sigint_handler(int sig){
    printf("\nWe got the signal: SIGINT!\n");
    printf("···· We should wait for the snooze return.\n");
}

unsigned int snooze(unsigned int secs){

    int selpt_secs = secs - sleep(secs);

    printf("Slept for %d of %d secs.\n", selpt_secs, secs);

    return secs - selpt_secs;
}