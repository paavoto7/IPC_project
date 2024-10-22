The task was to implement inter-process communication between two processes init and Scheduler.

## Implementation Criteria:
The project task should be implemented following the criteria below.
Criteria 1:
(a). init is a parent process, which forks child processes P1-P4. Use fork system call for this task.
(b). P1-P4: Process should generate a random integer number between 0-19. You may use a random
number generator or simply hardcode a number.
(c). P1-P4 should send the random number to init through message passing. Use pipe system call for
this task.
(d). init should read data sent by P1-P4.
Criteria 2:
(a). Scheduler is an independent process. Scheduler process should create a shared memory segment.
Use shmget and related system calls for this task.
(b). Scheduler should read data (list of random numbers) written by init, sort the list in ascending
order, and print the list. Use any sorting algorithm of choice.
Criteria 3:
(a). init should attach to the shared memory created by the scheduler process. Use shmat and related
system calls for this. Use any key of your choice as long as it is a unique key i.e., you may hardcode a
unique key value (notice that ftok system call is a clean way to do it, this your design choice).
(b). init should write random numbers from P1-P4 into shared memory.
Termination:
After scheduler prints the data, init should detach from shared memory, scheduler should detach and
delete the shared memory segment. Use shmdt and shmctl system calls for this task.
