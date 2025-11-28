#include <stdio.h>

void report(int procnum, long value) {
  printf("Ring proc %d received value %ld.\n",procnum,value);
  fflush(stdout);
}