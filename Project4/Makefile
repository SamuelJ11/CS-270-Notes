GCC = gcc
CFLAGS = -Wall -g

all:    driver ringproc

driver: ringofprocs.h ringdriver.c
        $(GCC) $(CFLAGS) -o ringdriver ringdriver.c

ringproc:  ringofprocs.h ringproc.c report.o
        $(GCC) $(CFLAGS) -o ringproc ringproc.c report.o