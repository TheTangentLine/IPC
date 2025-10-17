#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

#define SHM_SIZE 1024

using namespace std;

int main()
{
    while (true)
    {
        cout << "Input the key: ";
        int smhKey;
        cin >> smhKey;

        int shmId;
        shmId = shmget(smhKey, SHM_SIZE, 0666);
        if (shmId == -1)
        {
            cout << "Wrong key, try again ahaha: " << endl;
            continue;
        }

        char *shm_ptr;
        shm_ptr = (char *)shmat(shmId, NULL, 0);
        if (shm_ptr == (char *)-1)
        {
            std::cerr << "Shmat failed" << endl;
            return 1;
        }

        const char *foundMsg = "Aha, I found it!";
        strncpy(shm_ptr, foundMsg, SHM_SIZE - 1);
        shm_ptr[SHM_SIZE - 1] = '\0';

        shmdt(shm_ptr);

        cout << "Congratulations! You won the game!";
        break;
    }

    return 0;
}