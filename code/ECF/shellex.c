#include <stdio.h>

#define MAXARGS 128
#define MAXLINE 128


void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
void Fgets(char *str, int n, FILE *stream)1

{
    fgets(str, n, stream);

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