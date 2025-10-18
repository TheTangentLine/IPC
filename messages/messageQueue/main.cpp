#include <iostream>
#include <cstring>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <csignal>
#include <thread>
#include <functional>
#include "message.hpp"

using namespace std;

// ----------------------------------- Global variables ------------------------------------->

int queueId = -1;
bool shouldExit = false;
bool inChannel = false;
long msgType = 1;
string currentUsername;

// ---------------------------------- Helper functions ------------------------------------>

void signalHandler(int signum);
void joinChannel(string &input, Message &message);
void receiveMessages();

// ---------------------------------- Main function -------------------------------->

int main()
{
    // =========== Set up program =============>
    signal(SIGINT, signalHandler);

    cout << "\n------------------------\n";
    cout << "Welcome to MessengerCLI!\n";
    cout << "------------------------\n\n";
    cout << "Enter your username: ";

    Message message;
    getline(cin, currentUsername);
    strncpy(message.username, currentUsername.c_str(), sizeof(message.username) - 1);
    message.username[sizeof(message.username) - 1] = '\0';
    message.mtype = msgType;

    cout << "\nWelcome, " << currentUsername << "!" << endl;
    cout << "Commands: '/join <channel>' to join a channel, '/quit' to exit" << endl
         << endl;

    // ========== Thread to collect messages ===========>
    thread receiverThread(receiveMessages);

    // ========== Run program ===========>
    while (!shouldExit)
    {
        cout << "> ";
        string input;
        getline(cin, input);

        if (input.empty())
            continue;

        else if (input == "/quit")
        {
            cout << "Goodbye!" << endl;
            break;
        }
        else if (input.substr(0, 6) == "/join ")
        {
            joinChannel(input, message);
        }
        else
        {
            cout << "Unknown command. Use '/join <channel>' or '/quit'" << endl;
        }
    }

    shouldExit = true;
    receiverThread.join();

    // Clean up message queue
    if (queueId != -1)
    {
        msgctl(queueId, IPC_RMID, nullptr);
    }

    return 0;
}

// --------------------------------- Function implementation ------------------------------->

// ========== Signal handler ===========>
void signalHandler(int signum)
{
    cout << "\n\nExiting...\n";
    shouldExit = true;
    exit(0);
}

// ========== Join channel ============>
void joinChannel(string &input, Message &message)
{
    string channel = input.substr(6);
    if (channel.empty())
    {
        cout << "Please specify a channel name." << endl;
        return;
    }

    key_t key = ftok(channel.c_str(), 65);
    queueId = msgget(key, 0666 | IPC_CREAT);

    if (queueId == -1)
    {
        cout << "Failed to create/join channel." << endl;
        return;
    }

    inChannel = true;
    cout << "Joined channel: " << channel << endl;
    cout << "Type messages to send, '/leave' to leave channel\n\n";

    while (!shouldExit)
    {
        cout << currentUsername << ": ";
        string msgText;
        getline(cin, msgText);

        if (msgText == "/leave")
        {
            inChannel = false;
            cout << "Left channel." << endl;
            break;
        }

        if (msgText.empty())
            continue;

        strncpy(message.data, msgText.c_str(), sizeof(message.data) - 1);
        message.data[sizeof(message.data) - 1] = '\0';

        if (msgsnd(queueId, &message, sizeof(Message) - sizeof(long), 0) == -1)
        {
            cout << "Failed to send message." << endl;
        }
    }
}

// ========== Receive messages ===========>
void receiveMessages()
{
    Message receiver;
    while (!shouldExit)
    {
        if (!inChannel)
        {
            this_thread::sleep_for(chrono::milliseconds(100));
            continue;
        }

        if (msgrcv(queueId, &receiver, sizeof(Message) - sizeof(long), msgType, IPC_NOWAIT) > 0)
        {
            if (strcmp(receiver.username, currentUsername.c_str()) != 0)
            {
                cout << "\r" << receiver.username << ": " << receiver.data << endl;
                cout << currentUsername << ": " << flush;
            }
            // else cout << "\r" << "This message has not been sent yet\n";
        }
        else
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
}