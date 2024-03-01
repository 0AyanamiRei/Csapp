#include <stdio.h>

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
        if(feof(stdin)){
            exit(0);
        }

        eval(cmdline);
    }
    
}