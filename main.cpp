#include <iostream>
#include <random>
#include <unistd.h>

using namespace std;

int randGen() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1,6);
    return dist(gen);
}

int main() {
    //pid_t pid = fork();

    int fd[2];
    pipe(fd);
    
    pid_t mainPid = getpid();

    //pid_t P2 = fork();
    //pid_t P1 = fork();
    //pid_t P3 = fork();
    //pid_t P4 = fork();

    for (int i = 0; i < 4; ++i) {

        if (getpid() == mainPid) {
            pid_t P = fork();
        }
    }

    if (getpid() != mainPid) {
        int randNum = randGen();
        cout << randNum << endl;
    }

    if (getpid())

    return 0;
}

