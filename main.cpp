#include <iostream>
#include <random>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>

using namespace std;

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
    cout << "Process: " << getpid() << ", number: " << randNum << endl;
    return 0;
}

int main() {

    cout << "Main process pid: " << getpid() << endl << endl;

    int fd[2];
    pipe(fd);

    key_t key = ftok("topSec", 1);
    int shmid = shmget(key, 4*sizeof(int), 0);

    int* shmAdr = (int*) shmat(shmid, NULL, 0);
    
    // Creates the P0-P4 processes
    for (int i = 0; i < 4; ++i) {
        pid_t pid = fork();

        // Waits for the child to be done
        if (pid != 0) {
            wait(NULL);
        } else if (pid == 0) {
            return childFunc(fd);
        }
    }

    cout << endl;

    int n;
    for (int i = 0; i < 4; ++i) {
        read(fd[0], &n, sizeof(n)); // Read pipe
        int addr = i * sizeof(n); // Address to write (avoid segfault)
        shmAdr[addr] = n; // shmAdr is a pointer so writing there
        cout << "Reading from pipe: " << n << endl;
    }

    cout << endl;

    shmdt(shmAdr);

    return 0;
}

