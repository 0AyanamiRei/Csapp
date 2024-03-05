#include "/home/refrain/csapp/Csapp/code/mycsapp.c"
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid;

    printf("Start\n");
    pid = Fork();
    printf("%d\n", !pid);
    if (pid == 0) {
        printf("Child\n");
    } else if ((waitpid(-1, &status, 0) > 0) && (WIFEXITED(status) != 0)) {
        printf("%d\n", WEXITSTATUS(status));
    }
    printf("Stop\n");
    exit(2);
}
