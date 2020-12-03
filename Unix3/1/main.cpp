//g++ main.cpp -o main -pthread -std=c++11

#include <pthread.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Status(int, const char*);

const useconds_t S1 = 1500000;//���
const useconds_t S2 = 1000000;//���

const size_t N = 10;

volatile size_t a[N];

void* thread(void*)
{
	//���������� ������� �������
	for(size_t i = 0; i < N; i++)
	{
		a[i] = i + 1;
		usleep(S1);
	}
    return 0;
}

int main(int argc, char** argv)
{
	//����� ���������� ��������� �������
	cout << "Initial state of the array:  ";
	for(size_t i = 0; i < N; i++)
		cout << a[i] << ' ' << flush;
	cout << endl;
    //������� �����
	pthread_t  hThread;
	int status;
	if(status = pthread_create(&hThread, nullptr, thread, nullptr)) 
        Status(status, "Can't create thread");
	//����� ����������������� ��������� �������
    usleep(S2);
    cout << "Modified state of the array: ";
	for(size_t i = 0; i < N; i++)
	{
		cout << a[i] << ' ' << flush;
		usleep(S2);
	}
	cout << endl;
	//����, ���� ����� thread �������� ���� ������
    if(status = pthread_join(hThread, nullptr)) 
        Status(status, "Can't join thread");
	//����� �������������� ��������� �������
    cout << "Final state of the array:    ";
	for(size_t i = 0; i < N; i++)
		cout << a[i] << ' ' << flush;
	cout << endl;
    //���� ������� �� ���������� ��������
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}

void Status(int status, const char* str)
{
	cout << str << ", status = 0x" << hex << status 
        << '(' << dec << status << ')'<< endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(status);
}