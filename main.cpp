#include <iostream>
#include <random>
#include <unistd.h>

using namespace std;

int main() {
    //pid_t pid = fork();

    int fd[2];
    pipe(fd);
    
    pid_t main = getpid();

    //pid_t P2 = fork();
    //pid_t P1 = fork();
    //pid_t P3 = fork();
    //pid_t P4 = fork();

    for (int i = 0; i < 4; ++i) {
        pid_t P = fork();
        if (P != main) {
           random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(1,6);
            int num = dist(gen);
            cout << num << endl;
        }
    }
}