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

const DWORD S1 = 10;//��
const DWORD S2 = 30;//��
const size_t N = 3;//�� ������ MAXIMUM_WAIT_OBJECTS

DWORD WINAPI thread(LPVOID arg)
{
	//������� ������
	for(size_t j = *static_cast<size_t*>(arg) * 10, max = j + 10; j < max; j++)
	{
		//������� ������ ����� j
		for(size_t i = 0; i < 10; i++)
		{
			cout << j << ' ' << flush;
			Sleep(S1);
		}
		cout << "thread" << *static_cast<size_t*>(arg) << endl;
		Sleep(S1*10);
	}
	return 0;
}

int main(int argc, char** argv)
{
	//������� ������
    HANDLE hThread[N];
    size_t arg[N];      
	for(size_t i = 0; i < N; i++)
	{
        arg[i] = i + 1;
        hThread[i] = CreateThread(nullptr, 0, thread, arg + i, 0, nullptr);
        if (hThread[i] == nullptr)
            Error("Can't create thread");
	}
	//������� ������
	for(size_t j = 0; j < 10; j++)
	{
		//������� ������ ����� j
		for(size_t i = 0; i < 10; i++)
		{
			cout << 0 << j << ' ' << flush;
			Sleep(S2);
		}
		cout << " main" << endl;
		Sleep(S2*10);
	}
	//���� ���������� �������
    DWORD status = WaitForMultipleObjects(N, hThread, true, INFINITE);
	if(status == WAIT_FAILED)
        Status(status, "Wait for threads failed");
	//��������� ����������� �������
	for(size_t i = 0; i < N; i++)
        if(!CloseHandle(hThread[i]))
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