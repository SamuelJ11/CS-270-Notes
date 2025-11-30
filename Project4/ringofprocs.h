#include <stdio.h>  /* for printf(), fgets(), etc. */
#include <unistd.h>  /* for pipe() and ... */
#include <sys/types.h> /* for pid_t */
#include <wait.h>  /* for wait() */
#include <stdlib.h> /* for atoi(), atol(), getenv() */
#include <errno.h>  /* for perror() */
#include <signal.h> /* for kill */
#include <stdint.h> /* for uint16_t */

#define RINGMAX 8
#define READFDARG 1
#define WRITEFDARG 2
#define READYFDARG 3

#define PROCNUMKEY "CHILDINDX"
#define NUMCYCSKEY "NUMCYCLES"
#define RINGPROC "./ringproc"

#define NUMBUFLEN 16
#define MAXCYCLES 8

void signal_ready(void);
void report(int procnum, long value);

