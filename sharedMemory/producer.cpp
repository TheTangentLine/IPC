#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstring>

#define SHM_SIZE 1024

using namespace std;

int main()
{
    cout << "You are the one who hide the treasure!" << endl;
    cout << "Tell me the password for the treasure, input a number: ";

    int shmKey;
    cin >> shmKey;

    int shmId = shmget(shmKey, SHM_SIZE, IPC_CREAT | 0666);
    if (shmId == -1)
    {
        cerr << "There are no other places to hide the treasure." << endl;
        return 1;
    }

    char *shmPtr = (char *)shmat(shmId, NULL, 0);
    if (shmPtr == (char *)-1)
    {
        cerr << "Shmat failed." << std::endl;
        return 1;
    }

    char *secretMessage = "Oops, you find it!";
    strcpy(shmPtr, secretMessage);

    cout << "You have hidden the treasure" << endl;

    const string message = "Waiting for someone to find";
    while (true)
    {
        cout << "\r" << message;
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            fflush(stdout);
            sleep(1);
        }
        cout << "\r" << message << "   ";
        fflush(stdout);

        if (strcmp(secretMessage, shmPtr) != 0)
        {
            cout << "\nYour treasure has been found!" << endl;
            sleep(1);
            cout << "Game over!";
            return 0;
        }
    }

    shmdt(shmPtr);
    shmctl(shmId, IPC_RMID, NULL);

    return 0;
}