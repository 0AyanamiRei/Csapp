#include "fork.c"

int main()
{
    if (Fork() == 0) {
        printf("a"); fflush(stdout);
    }
    else {
        printf("b"); fflush(stdout);
        waitpid(-1, NULL, 0);
    }
    printf("c"); fflush(stdout);
    exit(0);
}

/*  use fflush(stdout)     093636 */
/*  not use fflush(stdout) 936036 */

/* why they are different？
   首先解释fflush(stdout)这条指令的含义: 情况标准输出缓存区，将所有未写入的数据发送到屏幕
   由于父进程和子进程都共享一个缓冲区，所以有可能printf的输出在父进程和子进程中都出现
   我们需要手动清空缓存区
*/