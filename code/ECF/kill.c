#include"csapp.c"

int main()
{
    pid_t pid;
    if((pid = Fork()) == 0){
        Pause();
        printf("signal never reached here. \n");
        exit(0);
    }

    kill(pid, SIGKILL);/* 注意windows没有提供kill函数 */
    
    return 0;
}