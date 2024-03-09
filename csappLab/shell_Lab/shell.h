#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string>
#include <vector>

#define stopped 0
#define running 1
#define MAXARGS 128
#define MAXLINE 128

struct proc{
    int pid;
    int pgid;
    int status;
    char *proc_args[];
};


volatile sig_atomic_t fg_pid;
volatile sig_atomic_t tmp_pid;

int builtin_command(char **argv);
int parseline(char *buf, char **argv);
