#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
    printf("eeeee\n");
    sleep(atoi(argv[1]));
    printf("Over!!\n");
    return 9; // 9>>8=2304
}