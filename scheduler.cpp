#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/unistd.h>

using namespace std;

/* At the moment this scheduler is using sleep to wait for the other file to be done.
It isn't really scheduling anything and the implementation is really clumsy.
If these were initiated from the same top process this could work better I think,
but the project specification states that scheduler is a "Scheduler is an independent process".
I would imagine this is something similar that was shown during a lecture by using two different
executables as entrypoints that exchanged data through pipe/FIFO.
This implementation seems to be working though so I suppose we're on the right tracks at least.
*/

int main() {

    key_t key = ftok("topSec", 1);

    int shmid = shmget(key, 4*sizeof(int), IPC_CREAT | 0666);

    sleep(1);

    int* shmAdr = (int*) shmat(shmid, NULL, 0);
    
    cout << "The numbers written are ";

    for (int i = 0; i < 4; ++i) {
        cout << shmAdr[i*sizeof(int)] << " ";
    }

    cout << endl;

    shmdt(shmAdr);

    shmctl(shmid, IPC_RMID, 0);

    return 0;
}

