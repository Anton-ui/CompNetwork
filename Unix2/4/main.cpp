//g++ main.cpp -o main -pthread -std=c++11

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */

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
const char * const name = "named_semaphore";
const size_t N = 3;

int main(int argc, char** argv)
{
    //������� �������� �������
    sem_t* sem = sem_open(name, O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, 1);
    if(sem == SEM_FAILED)
        Error("Create semaphore failed");
    //��������� �������� �������s
    for(size_t k = 0; k < N; k++)
        switch(fork())
        {
            case -1:
                     Error("The new process is not created");
            case 0:
                    //�������
                    //������� ������
                    for(size_t j = (k + 1) * 10, max = j + 10; j < max; ++j)
                    {
                        //����������� �������
                        if(sem_wait(sem))
                            Error("Wait for semaphore failed");
                        //������� ������ ����� j
                        for(size_t i = 0; i < 10; ++i)
                        {
                            cout << j << ' ' << flush;
                            usleep(S1);
                        }
                        cout << "child" << k + 1 << endl;
                        //����������� �������
                        if(sem_post(sem))
                            Error("Can't release semaphore");
                        //
                        usleep(S1*10);
                    }
                    //��������� �������
                    if(sem_close(sem))
                        Error("Close semaphore failed");
                    return 0;
        }
    //��������
    //������� �� ����� ������
    for(size_t j = 0; j < 10; ++j)
    {
        //����������� �������
        if(sem_wait(sem))
            Error("Wait for semaphore failed");
        //������� ������ ����� j
        for(size_t i = 0; i < 10; ++i)
        {
            cout << 0 << j << ' ' << flush;
            usleep(S2);
        }
        cout << "parent" << endl;
        //����������� �������
        if(sem_post(sem))
            Error("Can't release semaphore");
        //
        usleep(S2*10);
    }
    //����, ���� ��� �������� �������� �������� ������
    for(size_t k = 0; k < N; k++)
        if(wait(nullptr) == ERROR)
            Error("Wait for process failed");
    //��������� �������
    if(sem_close(sem))
        Error("Close semaphore failed");
    if(sem_unlink(name))
        Error("Unlink semaphore failed");
    //���� ������� �� ���������� ��������
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