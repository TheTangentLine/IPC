#include <iostream>
#include <signal.h>

using namespace std;

int main()
{
    int pid;
    while (true)
    {
        cout << "Input the process id: ";
        cin >> pid;

        if (kill(pid, SIGTERM) != -1)
        {
            cout << "Seen..." << endl
                 << endl;
        }
    }

    return 0;
}