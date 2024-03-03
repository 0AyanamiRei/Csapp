/* $begin waitpid1 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#define N 2
#define LEN 100
typedef void (*sighandler_t)(int);
extern char etext; /* &etext会获取只读文本段的结束地址 */
char *ptr = &etext + 0x8;

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

sighandler_t Signal(int signum, sighandler_t handler)
{
    if (signal(signum, handler) == SIG_ERR)
        unix_error("signal error");
}


int main() {
  int status, i;
  pid_t pid;

  /* Parent creates N children */
  for (i = 0; i < N; i++)
    if ((pid = Fork()) == 0) {
      /* access address 0, cause fault */
      char* cptr = NULL;
      *cptr = 'd';
    }

  /* Parent reaps N children in no particular order */
  while ((pid = waitpid(-1, &status, 0)) > 0) {
    if (WIFEXITED(status))
      printf("child %d terminated normally with exit status=%d\n",
          pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) {
      /* print signal that cause process exit */
      char buf[LEN];
      sprintf(buf, "child %d terminated by signal %d", pid, WTERMSIG(status));
      psignal(WTERMSIG(status), buf);
    }
    else
      printf("child %d terminated abnormally\n", pid);
  }

  /* The only normal termination is if there are no more children */
  if (errno != ECHILD)
    unix_error("waitpid error");

  exit(0);
}
/* $end waitpid1 */
