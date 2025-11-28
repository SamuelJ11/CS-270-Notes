#include <stdio.h>
#include "ringofprocs.h"

/* this is bigger than necessary */
#define SMALLNUMLEN 8
#define ENVSTRINGLEN 64

typedef struct pipends {
  int read_end;
  int write_end;
} pipends_t;

int main(int argc, char *argv[]) {
  int numprocs, numcycles;
  int pipearg[2];  /* in-out param for pipe() */
  pipends_t *ringpipes;
  pipends_t readypipe;
  pid_t *childpids;
  int i,j, leader, rv;
  uint16_t childmap, inmap;
  uint8_t childid;
  char childindxstr[ENVSTRINGLEN];
  char numcycstr[ENVSTRINGLEN];
  char inbuf[SMALLNUMLEN];  // for inputting the leader #
  char infdstr[SMALLNUMLEN];
  char outfdstr[SMALLNUMLEN];
  char readyfdstr[SMALLNUMLEN];
  char *envp[] = { childindxstr, NULL };
  /* only the leader gets the number of cycles in its environment */
  char *leaderenvp[] = { childindxstr, numcycstr, NULL };
  char *pargv[] = { RINGPROC, infdstr, outfdstr, readyfdstr, NULL };

  if (argc != 3) {
    fprintf(stderr,"Usage: %s <ring size> <#cycles>\n",argv[0]);
    exit(1);
  }

  numprocs = atoi(argv[1]);
  if (numprocs <= 0) {
    fprintf(stderr,"Ring size must be positive and at most %d.\n",RINGMAX);
    exit(2);
  }

  numcycles = atoi(argv[2]);
  if (numcycles <= 0 || numcycles > MAXCYCLES) {
    fprintf(stderr,"Sorry, # of cycles must be positive and at most %d.\n",
            MAXCYCLES);
    exit(3);
  }
  snprintf(numcycstr,ENVSTRINGLEN,NUMCYCSKEY "=%d",numcycles);

  leader = -1;
  while (leader < 0 || leader > (numprocs-1)) {
    printf("Enter a process number in [0-%d] to be leader: ",numprocs-1);
    if (fgets(inbuf,SMALLNUMLEN,stdin)==NULL) {
      printf("no input, exiting.\n");
      kill(0,SIGTERM);
      exit(10);
    }
    leader = atoi(inbuf);
  }
  printf("child %d will be leader.\n",leader);

  /* create the ready pipe */
  if (pipe(pipearg) < 0) {
    perror("pipe failed:");
    exit(4);
  }
  readypipe.read_end = pipearg[0];
  readypipe.write_end = pipearg[1];
  snprintf(readyfdstr,SMALLNUMLEN,"%d",readypipe.write_end);

  /* space for the pipe fds. We have to create them all at once. */
  ringpipes = calloc(numprocs,sizeof(pipends_t));
  for (i=0; i<numprocs; i++) {
    if (pipe(pipearg) < 0) {
      perror("pipe failed:");
      exit(5);
    }
    ringpipes[i].read_end = pipearg[0]; /* read end */
    ringpipes[i].write_end = pipearg[1]; /* write end */
  }

  /* array to save the child pids in */
  childpids = calloc(numprocs,sizeof(pid_t));

  /* create processes, set up their args and environments and
   * CLOSE THE OTHER (unused) FILE DESCRIPTORS.
   */
  for (i=0; i<numprocs; i++) {
    /* set up args and env for process i */
    snprintf(infdstr,SMALLNUMLEN,"%d",ringpipes[i].read_end);
    snprintf(outfdstr,SMALLNUMLEN,"%d",ringpipes[(i+1)%numprocs].write_end);
    snprintf(readyfdstr,SMALLNUMLEN,"%d",readypipe.write_end);
    snprintf(childindxstr, ENVSTRINGLEN, PROCNUMKEY "=%d",i);

    /* fork off child i */
    if ((childpids[i] = fork()) < 0) {
      perror("fork failed");
      exit(6);
    }

    if (childpids[i]==0) { /* child - set input/output and overlay self */
      /* close unneeded file descriptors */
      for (j=3; j<4*numprocs+3; j++)
        if ((j != ringpipes[i].read_end) &&
            (j != ringpipes[(i+1)%numprocs].write_end) &&
            (j != readypipe.write_end))
          close(j);

      /* now exec the ring [student's] ring process code */
      if (i==leader)
        rv = execve(RINGPROC,pargv,leaderenvp); // only leader gets numcycles
      else
        rv = execve(RINGPROC,pargv,envp);
      if (rv<0) {
        perror("execve failed");
        exit(7);
      }
      /* NOTREACHED */
    } /* child */
  } /* for each process */

  /* parent: close unneeded file descriptors! */
  for (i=0; i<numprocs; i++) {
    close(ringpipes[i].read_end);
    close(ringpipes[i].write_end);
  }
  close(readypipe.write_end);

  /* now wait for indication that each child is ready */
  /* set up bitmap */
  childmap = 0xffff;
  childmap <<= numprocs;
  childmap = ~childmap;  // low order bit set for each child
  inmap = 0;
  while (inmap != childmap) {
    if ((i=read(readypipe.read_end,&childid,1)) > 0) {
      inmap |= (1 << childid);
      printf("Child %d is ready.\n",childid);
    } else if (i==0) {
      fprintf(stderr,"Premature EOF on ready pipe, inmap=%x\n.",inmap);
      exit(8);
    } else {
      perror("read on ready pipe");
      exit(9);
    }
  }
  /* all children have reported in */

  if (kill(childpids[leader],SIGUSR1) < 0) {
    perror("kill");
    exit(11);
  }

  /* now wait for all the kids to terminate */
  j = 0;
  while (j < numprocs) {
    if (wait(NULL) > 0) {
      j++;
    } else {
      perror("wait");
    }
  }

  printf("All done!\n");
  exit(0);
}