#include <iostream>
#include <cstring>
using namespace std;

struct Message
{
    long mtype;
    char username[50];  // Fixed: Use char array instead of std::string for POD compatibility
    char data[100];
};