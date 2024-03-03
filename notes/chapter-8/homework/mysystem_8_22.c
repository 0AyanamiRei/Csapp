/* Read me!
 * 编写一个Unix system函数:
 * int mySystem(char *command);
 * mySystem通过调用"/bin/sh -c command"来执行command, 然后在command完成后返回
 * 如果command正常退出, 那么mySystem返回其退出状态
 * 否则,mySystem返回shell返回的状态
 *
 */

/*** Just try and believe yourself !! ***/


/* 调用/bin/sh -c, 也就是说argv[0] = "/bin/sh", argv[1] = "-c", argv[2]是指令名称*/

/* 问题是提到的 "shell返回的状态" */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern **environ;

int mySystem(char *command){
    pid_t pid;
    if((pid = fork()) == 0){ /* child process*/
        char *argv[] = {"/bin//sh", "-c", command, NULL};
        execve(argv[0], argv, environ);
    } else { /* parent process */
        int status;
        waitpid(pid, &status, 0); /* reap the child process */

        if(WIFEXITED(status)){
            return WEXITSTATUS(status);
        }
        return status;
    }
    return -1;
}

int main(int argc, char **argv, char **envp){

    char cmd[256];
    printf("Enter what command you wang to execute: ");
    gets(cmd);
    printf("You want to execute this command : %s\n", cmd);

    printf("The shell return %d\n", mySystem(cmd));
    exit(0);
}