#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define MAXARGS 128
#define MAXLINE 128

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
char *Fgets(char *str, int n, FILE *stream)
{
    char *res = fgets(str, n, stream);
    if(res == NULL){
        printf("error: fgets读取失败!");
        exit(0);
    }
    return res;
}

int main()
{
    char cmdline[MAXLINE];

    while (1)
    {
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if(feof(stdin)){ /* file end of file 检查用户是否输入结束符 (Ctrl+D或Ctrl+Z) */
            exit(0);
        }

        eval(cmdline);
    }
    
}
void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;
    


}