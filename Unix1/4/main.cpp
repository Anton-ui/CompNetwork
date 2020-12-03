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

const useconds_t S1 = 10000;//мкс
const useconds_t S2 = 30000;//мкс
const size_t N = 3;

int main(int argc, char** argv)
{
    for(size_t k = 0; k < N; k++)
        switch(fork())
        {
            case -1:
                     Error("The new process is not created");
            case 0:
                    //Потомок
                    //Выводим данные
                    for(size_t j = (k + 1) * 10, max = j + 10; j < max; ++j)
                    {
                        //Выводим строку чисел j
                        for(size_t i = 0; i < 10; ++i)
                        {
                            cout << j << ' ' << flush;
                            usleep(S1);
                        }
                        cout << "child" << k + 1 << endl;
                        usleep(S1*10);
                    }
                    return 0;
        }
    //Родитель
    //Выводим на экран данные
    for(size_t j = 0; j < 10; ++j)
    {
        //Выводим строку чисел j
        for(size_t i = 0; i < 10; ++i)
        {
            cout << 0 << j << ' ' << flush;
            usleep(S2);
        }
        cout << "parent" << endl;
        usleep(S2*10);
    }
    //Ждем, пока все дочерние процессы закончат работу
    
    for(size_t k = 0; k < N; k++)
        if(wait(nullptr) == ERROR)
            Error("Wait for process failed");
    cout << "Press enter to exit." << endl;
    cin.get();
    return 0;
}

void Error(const char* str)
{
    cout << str << ", errno = 0x" << hex << errno
        << '(' << dec << errno << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(errno);
}