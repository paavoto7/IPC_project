#include <iostream>
#include <random>
#include <unistd.h>

using namespace std;

int randGen() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(0,19);
    return dist(gen);
}

int childFunc(int fd[]) {
    int randNum = randGen();
    write(fd[1], &randNum, sizeof(randNum));
    return 0;
}

int main() {

    int fd[2];
    pipe(fd);
    
    pid_t mainPid = getpid();

    for (int i = 0; i < 4; ++i) {

        if (fork() == 0) {
            return childFunc(fd);
        }
    }

    int n;

    for (int i = 0; i < 4; ++i) {
        read(fd[0], &n, sizeof(n));
        cout << n << endl;
    }

    return 0;
}

