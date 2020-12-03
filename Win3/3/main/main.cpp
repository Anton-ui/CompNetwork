//c++ 11

#include <windows.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Error(const char*);
void Status(DWORD, const char*);

const DWORD S1 = 1500;//��
const DWORD S2 = 1000;//��

const size_t N = 10;

volatile size_t a[N];

HANDLE hSemaphore;

DWORD WINAPI thread(LPVOID)
{
	//���������� ������� �������
	for(size_t i = 0; i < N; i++)
	{
		a[i] = i + 1;
        //��������, ��� ���� ������� �����
		if(!ReleaseSemaphore(hSemaphore,1,NULL))
            Error("Can't release semaphore");
        //
		Sleep(S1);
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
	//������� �������    
	hSemaphore=CreateSemaphore(nullptr, 0, N, nullptr);
	if(hSemaphore == nullptr)
		Error("Can't create semaphore");
    //������� �����, ������� ������� �������� �������
	HANDLE hThread = CreateThread(nullptr, 0, thread, nullptr, 0, nullptr);
	if(hThread == nullptr)
		Error("Can't create thread");
	//����� ����������������� ��������� �������
    Sleep(S2);
	cout << "Modified state of the array: ";
	for(size_t i = 0; i < N; i++)
	{
		//�������� ���������� ������
        DWORD status = WaitForSingleObject(hSemaphore, INFINITE);
        if(status != WAIT_OBJECT_0)
    		Status(status, "Wait for semaphore failed");
        //
        cout << a[i] << ' ' << flush;
		Sleep(S2);
	}
	cout << endl;
	//���� ���������� ������ thread
	DWORD status = WaitForSingleObject(hThread, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for thread failed");
	//����� �������������� ��������� �������
    cout << "Final state of the array:    ";
	for(size_t i = 0; i < N; i++)
		cout << a[i] << ' ' << flush;
	cout << endl;
	//��������� ���������� ������ thread
	if(!CloseHandle(hThread))
		Error("Can't close handle");
    //��������� ���������� ��������
	if(!CloseHandle(hSemaphore))
		Error("Can't close handle");
    //���� ������� �� ���������� ��������
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}

void Error(const char* str)
{
	DWORD error = GetLastError();
	cout << str << ", error = 0x" << hex << error 
		<< '(' << dec << error << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(error);
}

void Status(DWORD status, const char* str)
{
	cout << str << ", status = 0x" << hex << status
		<< '(' << dec << status << ')'<< endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(status);
}