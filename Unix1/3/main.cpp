//g++ main.cpp -o main -std=c++11

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Error(const char*);

const pid_t ERROR = -1;

const useconds_t S1 = 10000;//���
const useconds_t S2 = 30000;//���

int main(int argc, char** argv)
{
    switch(fork())
    {
        case -1:
                Error("The new process is not created");
        case 0:
                //�������
                //������� ������
                for(size_t j = 10; j < 20; ++j)
                {
                    //������� ������ ����� j
                    for(size_t i = 0; i < 10; ++i)
                    {
                        cout << j << ' ' << flush;
                        usleep(S1);
                    }
                    cout << "child" << endl;
                    usleep(S1*10);
                }
                return 0;
        default:
                //��������
                //������� �� ����� ������
                for(size_t j = 0; j < 10; ++j)
                {
                    //������� ������ ����� j
                    for(size_t i = 0; i < 10; ++i)
                    {
                        cout << 0 << j << ' ' << flush;
                        usleep(S2);
                    }
                    cout << "parent" << endl;
                    usleep(S2*10);
                }
                if(wait(nullptr) == ERROR)
                    Error("Wait for process failed");
                cout << "Press enter to exit." << endl;
                cin.get();
                return 0;
    }
}

void Error(const char* str)
{
    cout << str << ", errno = 0x" << hex << errno
        << '(' << dec << errno << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(errno);
}