#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string>
#include <vector>

#define MAXARGS 128
#define MAXLINE 128
#define TRUE 1
#define FALSE 0
#define LEN 64
#define Stopped 0
#define Running 1

#ifndef SHELL_H
#define SHELL_H

using namespace std;

volatile sig_atomic_t fg_pid;
volatile sig_atomic_t tmp_pid;

#endif // SHELL_H
extern char **environ;

struct Job
{
    sig_atomic_t pid;
    int status;
    char **command;
};

vector<Job> Jobs;



/****************************************/
/************pre function****************/
/****************************************/
void unix_error(char *msg);
char *Fgets(char *ptr, int n, FILE *stream);
sighandler_t Signal(int signum, sighandler_t handler);
void print_String(char *message);
void print_int(int value);


/****************************************/
/************main function***************/
/****************************************/
void eval(char *cmdline);
int parseline(char *buf, int& argc, char **argv);
int builtin_command(char **argv);
int jobs(char **argv);
int bg(char **argv);
int fg(char **argv);



/****************************************/
/*******signal handler function**********/
/****************************************/
void SIGCHLD_handler(int sig);
void SIGINT_handler(int sig);
void SIGTSTP_handler(int sig);
void SIGUSR1_handler(int sig);


                /****************************************/
                /*******jobs' data struct**********/
                /****************************************/
void initjobs();
void addjobs(sig_atomic_t pid);