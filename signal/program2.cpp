#include <iostream>
#include <unistd.h>

using namespace std;

int cnt = 1;

void signalHandler(int signum)
{
    cout << "\r\033[K(" << cnt++ << ") " << "Aha, I receive the signal.\n";
}

int main()
{
    signal(SIGTERM, signalHandler);
    cout << "Process ID: " << getpid() << endl;

    const string message = "Waiting";
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
    }

    return 0;
}