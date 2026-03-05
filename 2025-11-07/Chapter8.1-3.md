# CHAPTER 8:  EXCEPTIONAL CONTROL FLOW

## Exceptions

    • Exceptions are a form of exception control flow (ECF) that are implemented partly by the hardware and partly by the operating system:

        - the details vary from system to system but the basic ideas are the same

    • An exception is an abrupt change in the control flow in response to some change in the processor's state

        - the change in state is known as an 'event'

    • When the processor detects that the event has occured, it makes an indirect procedure call through a jump table called an 'exception table'

        - this jump is done to an operating system subroutine (the exception handler) that is specifically designed to process this kind of event

    • When the exception handler finishes, one of three things happen:

        (1) the hander returns control to the current instructions

        (2) the handler returns control to the next instruction

        (3) the handler aborts the interrupted program

### Exception Handling

    • At system boot time, the operating system allocates and initializes a jump table called an exception table, so that entry 'k' contains the address of the handler for exception 'k'.

        - entry 'k' contains the address of the handler code for exception 'k'

    • Each type of possible exception in a system is assigned a unique non-negative integer exception number, which is the index into the exception table (see figure 8.2-3 page 751)

    • The CPU keeps a special register called the 'exception table base register' (ETBR) that holds the starting address of the exception table in memory:

        - when the processor triggers an exception by making an indirect procedure call through entry k of the exception table, the processor forms the address of the appropriate exception handler using the formula below:

            address of handler = contents of memory at ETBR + (k * 8)

    • When an exception occurs, the processor saves the current execution state before jumping to the handler. 
    
    • The processor pushes a return address (either the current or next instruction, depending on the exception type) and other key information such as the EFLAGS register onto the stack. 
    
        - if the exception transfers control from a user program to the kernel, this data is pushed onto the kernel stack

    • After the handler has processed the event, it optionally returns to the interrupted program by executing a special “return from interrupt” instruction:

        - this instruction tells the CPU to pop the saved state (program counter, flags, etc.) from the stack, wwitch back to user mode if needed, and resume execution at the saved return address

### Classes of Exceptions

    • Exceptions can be divided into four classes: interrupts, traps, faults, and aborts:

        +-----------+------------------------------+-------------+-------------------------------------+
        | Class     | Cause                        | Async/sync  | Return behavior                     |
        +===========+==============================+=============+=====================================+
        | Interrupt | Signal from I/O device       | Async       | Always returns to next instruction  |
        +-----------+------------------------------+-------------+-------------------------------------+
        | Trap      | Intentional exception        | Sync        | Always returns to next instruction  |
        +-----------+------------------------------+-------------+-------------------------------------+
        | Fault     | Potentially recoverable error| Sync        | Might return to current instruction |
        +-----------+------------------------------+-------------+-------------------------------------+
        | Abort     | Nonrecoverable error         | Sync        | Never returns                       |
        +-----------+------------------------------+-------------+-------------------------------------+


    • I/O devices trigger interrupts by signaling a pin on the processor chip and placing on to the system bus the exception number that identifies the device that caused the interrupt.

        - the processor then reads the exception number from the system bus, and then calls the appropriate interrupt handler

    • Traps occurs when a program executes a particular instruction that asks the operating system (via a system call) to do something that user code itself isn’t allowed to do directly (read a file, start a new process, or exit)

    • Faults result from error conditions that a handler might be able to correct.

        - control is always returned to the faulting instruction

    • Aborts result from unrecoverable fatal errors

### Exceptions in Linux/x86-64 Systems

    • There can be up to 256 exception types. Numbers 0–31 are fixed by the Intel architecture for x86-64 systems, while numbers 32–255 are defined by the operating system for interrupts and traps:

        (1) Divide error (exception 0): Unix does not attempt to recover from divide erros, and Linux shells typically report divide erros as "Floating exceptions"

        (2) General protection fault (exception 13): a program references an undefined area of virtual memory or the program attempts to write to a read-only text segment. Linux shells report these as "segmentation faults"

        (3) Page fault (exception 14): programs use virtual memory addresses, which are mapped to physical memory (RAM).  
        
            -- when a program tries to access a virtual memory page that isn’t currently in physical memory, the CPU triggers a page fault exception 
            
            -- the page fault handler loads the page into physical memory and updates the page table, so the virtual address now points to the correct 
                physical memory location
                
            -- Once the page is in RAM and the mapping is updated, the CPU re-executes the instruction that caused the page fault

        (4) Machine check (exception 18): fatal hardware error detected during the execution of the faulting instruction

    • C programs can invoke any system call directly by using the syscall function, but this is rarely necessary in practice. 
    
        - the C standard library provides a set of convenient wrapper functions for most system calls

    • System calls and their associated wrapper functions will be referred to interchangeably as 'system-level functions'.

        +------------------+------------------+-------------------------------------+
        | System Call      | Name             | Description                         |
        | Number           |                  |                                     |
        +==================+==================+-------------------------------------+
        | 0                | read             | Read file                           |
        +------------------+------------------+-------------------------------------+
        | 1                | write            | Write file                          |
        +------------------+------------------+-------------------------------------+
        | 2                | open             | Open file                           |
        +------------------+------------------+-------------------------------------+
        | 3                | close            | Close file                          |
        +------------------+------------------+-------------------------------------+
        | 4                | stat             | Get info about file                 |
        +------------------+------------------+-------------------------------------+
        | 9                | mmap             | Map memory page to file             |
        +------------------+------------------+-------------------------------------+
        | 12               | brk              | Reset the top of the heap           |
        +------------------+------------------+-------------------------------------+
        | 32               | dup2             | Copy file descriptor                |
        +------------------+------------------+-------------------------------------+
        | 33               | pause            | Suspend process until signal arrives|
        +------------------+------------------+-------------------------------------+
        | 37               | alarm            | Schedule delivery of alarm signal   |
        +------------------+------------------+-------------------------------------+
        | 39               | getpid           | Get process ID                      |
        +------------------+------------------+-------------------------------------+
        | 57               | fork             | Create process                      |
        +------------------+------------------+-------------------------------------+
        | 59               | execve           | Execute a program                   |
        +------------------+------------------+-------------------------------------+
        | 60               | _exit            | Terminate process                   |
        +------------------+------------------+-------------------------------------+
        | 61               | wait4            | Wait for a process to terminate     |
        +------------------+------------------+-------------------------------------+
        | 62               | kill             | Send signal to a process            |
        +------------------+------------------+-------------------------------------+

    • All arguments to Linux system calls are passed through general-purpose registers rather than the stack. 
    
        - by convention, %rax contains the system call number, and up to six arguments are placed in %rdi, %rsi, %rdx, %r10, %r8, and %r9

    • Consider the code below: 

        #include <unistd.h>

        int main() {
            write(1, "hello, world\n", 13);
            _exit(0);
        }

        - the first argument to write sends the output to std out

        - the second argument is the sequence of bytes to write
        
        - the third argument specifies the number of bytes to write

## Processes

    • Exceptions are the basic building blocks that allow the operating system kernel to provide the notion of a process, one of the most profound and successful ideas in computer science.

    • The classic definition of a process is an instance of a program in execution, and its context (code, memory, registers, program counter, stack, environment, and open files) is everything the OS needs to execute or resume it correctly.

### Logical Control flow

    • Processes take turns using the processor, and each process is preempted (temporarily suspended) while other processes take thier turns.

### Concurrent Flows 

    • A logical flow whose execution overlaps in time with another flow is called a 'concurrent flow'.

    • The general phenomenon of multiple flows executing concurrently is known as 'concurrency' aka 'multitasking'.

    • If two flows are running concurrently on different processor cores or computers, then we say that they are 'parallel' flows

        - quick analogy:

            concurrency: like a single cook juggling multiple dishes — switching between them to make progress on all

            parallelism: like multiple cooks each working on a dish at the same time

### Private Address Space

    • A process provides each program with its own private address space. 
    
        - this space is private in the sense that a byte of memory associated with a particular address in the space cannot in general be read or written by any other process.

    • See figure 8.13 on page 761 to understand the discussion below:

        - the bottom portion of the address space is reserved for the user program, including the usual code, data, heap, and stack segments

        - rhe top portion of the address space is reserved for the kernel

### User and Kernel Modes

    • In order for the operating system kernel to provide an airtight process abstraction, the processor must provide a mechanism that restricts the instructions that an application can execute, as well as the portions of the address space that it can access.

        - processors typically provide this capability with a mode bit in a control register: 

            -- when the mode bit is set, the process is running in kernel mode

            -- when the mode bit is not set, the process is running in user mode

    • Linux provides a clever mechanism, called the /proc filesystem, that allows user mode processes to access the contents of kernel data structures

        - both /proc and /sys are interfaces to the kernel for user programs

        - they export kernel data as readable files, making it easy to inspect system state or hardware without directly accessing kernel memory

### Context Switches

    • The context switch mechanism is built on top of the lower-level exception mechanism

    • The kernel maintains a context (state that the kernel needs to restart a preempted process) for each process.
    
    • At certain points during the execution of a process, the kernel can decide to preempt the current process and restart a previously 
        preempted process. 
        
        - this decision is known as 'scheduling' and is handled by code in the kernel, called the 'scheduler'.

    • After the kernel has scheduled a new process to run, it preempts the current process and transfers control to the new process using a mechanism called a context switch, which:

        (1) Saves the context of the current process

        (2) Restores the saved context of a previously preempted process

        (3) Passes control to this newly restored process

    • All systems have some mechanism for generating periodic timer interrupts, typically every 1ms or 10ms.

        - each time a timer interrupt occurs, the kernel can decide that the current process has run long enough and switch to a new process

    • See figure 8.14 on page 763 for a visual of the anatomy of a process context switch.


##  System Call Error Handling

    • When Unix system-level functions encounter an error, they typically return -1 and set the global integer variable 'errno' to indicate what went wrong:  

        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error: %s\n", strerror(errno));
            exit(0);
        }
        
        - The strerror function returns a text string that describes the error associated with a particular value of 'errno'

    • We can a define a helper function to help simplify our code:

        void unix_error(char *msg) 
        {
            fprintf(stderr, "%s: %s\n", msg, strerror(errno));
            exit(0);
        }

    • Using the helper funciton, our call to fork reduces from 4 lines to 2:

        if ((pid = fork()) < 0)
        unix_error("fork error");

    • A wrapper function is just a normal function that calls another, but includes built-in error checking (and often error reporting).

        - we can use the Fork() wrapper function below: 

        pid_t Fork(void) {
        pid_t pid;

        if ((pid = fork()) < 0)
            unix_error("Fork error");

        return pid;
        }

        - given this wrapper, our call to fork shrinks to a single compact line:

            pid=Fork();

    • In all future discussion, wrapper functions will be referred to using thier capitalized notation, while the system-level functions will use lower case.


        









    
