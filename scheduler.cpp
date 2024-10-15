#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/unistd.h>

using namespace std;

/* Currently uses busy waiting in the form of having additional int that is written
at the end. We can't use zero as the indicator e.g. the last one, because the random
numbers are between 0 and 19.
*/

void sort(int nums[]) {
    // Simple and easy bubble sort

    bool swapped;

    do {
        swapped = false;
        for (int i = 0; i < 3; ++i) {
            if (nums[i] > nums[i+1]) {
                int temp = nums[i+1];
                nums[i+1] = nums[i];
                nums[i] = temp;
                swapped = true;
            }
        }
    } while (swapped);
}

int main() {

    key_t key = ftok("./key.txt", 2024);

    int shmid = shmget(key, 5*sizeof(int), IPC_CREAT | 0666);

    int* shmAdr = (int*) shmat(shmid, NULL, 0);

    cout << "Shared memory creted by: " << getpid() << endl;

    // Busy waiting as this is easy solution
    while (shmAdr[4] != -1) {
        sleep(0.2); // Reduce cpu usage
    }
    
    cout << "The numbers from shared memory are: ";

    int nums[4];

    for (int i = 0; i < 4; ++i) {
        int num = shmAdr[i];
        cout << num << " ";
        nums[i] = num;
    }

    cout << endl << "The sorted numbers are: ";
    
    sort(nums);
    for (int i = 0; i < 4; ++i) {
        cout << nums[i] << " ";
    }

    cout << endl;

    shmdt(shmAdr);

    shmctl(shmid, IPC_RMID, 0);

    return 0;
}

