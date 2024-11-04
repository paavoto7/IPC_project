The task was to implement inter-process communication between two processes init and Scheduler.

## Implementation Criteria:
The project task should be implemented following the criteria below.
### Criteria 1:
- (a). init is a parent process, which forks child processes P1-P4. Use fork system call for this task.
- (b). P1-P4: Process should generate a random integer number between 0-19. You may use a random
number generator or simply hardcode a number.
- (c). P1-P4 should send the random number to init through message passing. Use pipe system call for
this task.
- (d). init should read data sent by P1-P4.
### Criteria 2:
- (a). Scheduler is an independent process. Scheduler process should create a shared memory segment.
Use shmget and related system calls for this task.
- (b). Scheduler should read data (list of random numbers) written by init, sort the list in ascending
order, and print the list. Use any sorting algorithm of choice.
### Criteria 3:
- (a). init should attach to the shared memory created by the scheduler process. Use shmat and related
system calls for this. Use any key of your choice as long as it is a unique key i.e., you may hardcode a
unique key value (notice that ftok system call is a clean way to do it, this your design choice).
- (b). init should write random numbers from P1-P4 into shared memory.
### Termination:
After scheduler prints the data, init should detach from shared memory, scheduler should detach and
delete the shared memory segment. Use shmdt and shmctl system calls for this task.

## Implementation notes
The following were mandatory as a part of the course submissions.
#### To run:
On Linux that has g++ and bash located in bin, the run.sh file can be used.
Otherwise compile both the scheduler.cpp and main.cpp, and then run the scheduler first so
the memory segment is created and the main actually works properly. The latter is overall
the better method as it correctly conveys the purpose of this project.
### File main.cpp:
Init/main process:
Acts as the process which forks the P1-P4 child processes, creates and reades from the pipe,
and attaches and writes to the shared memory. In more detail:
It creates the pipe, attaches to the shared memory, forks the child processes in a loop, waits
for the children to exit, reads the random numbers from the pipe and writes them to the
shared memory. After that it writes additional -1 to the fifth position in the shared memory
to signify that the writing has been successfully completed and detaches from the shared
memory.
#### Child processes P1-P4:
They enter an if clause when created and there invoke a function called childFunc. This
function first calls a dedicated function for random number generation, then writes to the
pipe and exits using the exit function. Nothing else in the main function is needed but the
childFunch invocation as that handles the termination of the children.
### File scheduler.cpp:
This file has only one process which is used to create the shared memory segment, read the
numbers written there by init, print them and after that close the shared memory segment.
For sorting of the numbers it uses a sort function in the same file that implements the
bubble sort algorithm.
The process waits for the init to write to the shared memory by utilising the so called
method of busy waiting. This is done by making the shared memory segment the size of 5
integers and having the init write -1 at the last position. Then a while loop run checking for
the last position to not equal -1. It goes to sleep for .2 seconds to reduce CPU load. The busy
waiting method allows us to start the scheduler process on its own and not worry about
running them precisely simultaneously. Should ideally have a max waiting time, but it prints
the pid so if the init fails the user can manually kill the process.
### Other notes on implementation:
All the files contain magic numbers that are mostly related to the amount of random
numbers. This could be changed pretty easily with a global variable or something more
robust, but we didn’t see it necessary in this case.
