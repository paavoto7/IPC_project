#include <iostream>
#include <random>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>

using namespace std;

int definedInts(int index) {
    /* The instruction state that "The reviewer will give you 4 random numbers for P1-P4"
    so this is used for that if the random number generator is not sufficient.
    */
    int nums[] = {18, 4, 2, 6}; // Just random nums
    return nums[index];
}

int randGen() {
    // static as they don't change
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(0,19);
    return dist(gen);
}

// This is just to declutter the main function
int childFunc(int fd[]) {
    int randNum = randGen();
    write(fd[1], &randNum, sizeof(randNum));
    close(fd[1]);
    cout << "Process: " << getpid() << ", number: " << randNum << endl;
    exit(0);
}

int main() {

    cout << "Main process pid: " << getpid() << endl << endl;

    pid_t childPids[4];

    int fd[2];
    pipe(fd);

    key_t key = ftok("./key.txt", 2024);
    int shmid = shmget(key, 4*sizeof(int), 0);

    int* shmAdr = (int*) shmat(shmid, NULL, 0);

    if (shmid == -1 || shmAdr == (int*) -1) {
        cout << "Problem with the shared memory" << endl;
        return 1;
    }
    
    // Creates the P1-P4 processes
    for (int i = 0; i < 4; ++i) {
        pid_t pid = fork();
        childPids[i] = pid;
        
        if (pid == 0) {
            childFunc(fd);
        }
    }

    close(fd[1]);
    
    // Waits for the child to be done
    for (int i = 0; i < 4; ++i) {
        pid_t expid = waitpid(childPids[i], NULL, 0);
        cout << "Child process has exited: " << expid << endl;
    }

    cout << endl;

    int n;
    for (int i = 0; i < 4; ++i) {
        read(fd[0], &n, sizeof(n)); // Read pipe
        shmAdr[i] = n; // Write to the shared memory
        cout << "Reading from pipe: " << n << endl;
    }

    shmAdr[4] = -1; // This is used for implementing busy waiting

    close(fd[0]);

    cout << endl;

    shmdt(shmAdr);

    return 0;
}

