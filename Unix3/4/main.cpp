//g++ main.cpp -o main -pthread -std=c++11

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
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

const useconds_t S1 = 1500000;//���
const useconds_t S2 = 1000000;//���
const char * const name = "named_semaphore";

const size_t N = 10;   // ������ �������

int main(int argc, char** argv)
{
    //������� ������ �����������
    volatile size_t *ptr;
    if((ptr = static_cast<volatile size_t*>(mmap(nullptr, N * sizeof(size_t), PROT_READ | PROT_WRITE,
            MAP_ANON | MAP_SHARED, -1, 0))) == MAP_FAILED)
        Error("Create file mapping failed");
    //�������������� ������ 
	cout << "Initial state of the array:  ";
    for(size_t i = 0; i < N; i++)
		cout << (ptr[i] = 0) << ' ' << flush;
	cout << endl;
    //������� �������
    sem_t* sem = sem_open(name, O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, 0);
    if(sem == SEM_FAILED)
        Error("Create semaphore failed");
    //������� �������� �������
    switch(fork())
    {
        case -1:
                Error("The new process is not created");
        case 0:
                //�������
                //��������� ������
                for(size_t i = 0; i < N; i++)
                {
                    ptr[i] = i + 1;
                    //��������, ��� ���� ������� �����
                    if(sem_post(sem))
                        Error("Can't release semaphore");
                    //
                    usleep(S1);
                }
                //������� ������ �����������
                if(munmap(const_cast<size_t*>(ptr), N * sizeof(size_t)))
                    Error("Unmap view failed");
                //��������� �������
                if(sem_close(sem))
                    Error("Close semaphore failed");
                return 0;
        default:
                //��������
                //������� ������ � �������
                usleep(S2);
                cout << "Modified state of the array: ";
                for(size_t i = 0; i < N; i++)
                {
                    //�������� ���������� ������
                    if(sem_wait(sem))
                        Error("Wait for semaphore failed");
                    //
                    cout << ptr[i] << ' ' << flush;
                    usleep(S2);
                }
                cout << endl;
                //���� ���������� ���������� �������
                if(wait(nullptr) == ERROR)
                    Error("Wait for process failed");
                //����� �������������� ��������� �������
                cout << "Final state of the array:    ";
                for(size_t i = 0; i < 10; i++)
                    cout << ptr[i] << ' ' << flush;
                cout << endl;
                //������� ������ �����������
                if(munmap(const_cast<size_t*>(ptr), N * sizeof(size_t)))
                    Error("Unmap view failed");
                //��������� �������
                if(sem_close(sem))
                    Error("Close semaphore failed");
                //������� �������
                if(sem_unlink(name))
                    Error("Unlink semaphore failed");
                //���� ������� �� ���������� ��������
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