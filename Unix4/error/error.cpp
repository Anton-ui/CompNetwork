//g++ -fPIC -c error.cpp
//g++ -shared -o liberror.so error.o

#include <iostream>
#include "error.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;

void Status(int status, const char* str)
{
	cout << str << ", status = 0x" << hex << status 
        << '(' << dec << status << ')'<< endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(status);
}

void Error(const char* str)
{
    cout << str << ", errno = 0x" << hex << errno
        << '(' << dec << errno << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(errno);
}
