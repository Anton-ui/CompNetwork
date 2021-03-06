//c++ 11

#include <windows.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

const DWORD S1 = 10;//��
const DWORD S2 = 30;//��

void Error(const char*);
void Status(DWORD, const char*);

DWORD WINAPI thread(LPVOID)
{
	//������� ������
	for(size_t j = 10; j < 20; ++j)
	{
		//������� ������ ����� j
		for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(S1);
		}
		cout << "thread" << endl;
		Sleep(S1*10);
	}
	return 0;
}

int main(int argc, char** argv)
{
	//������� �����
	HANDLE hThread = CreateThread(nullptr, 0, thread, nullptr, 0, nullptr);
	if (hThread == nullptr)
        Error("Can't create thread");
	//������� ������
	for(size_t j = 0; j < 10; ++j)
	{
		for(size_t i = 0; i < 10; ++i)
		{
			//������� ������ ����� j
			cout << 0 << j << ' ' << flush;
			Sleep(S2);
		}
		cout << " main" << endl;
		Sleep(S2*10);
	}
	//���� ���������� ������ thread
	DWORD status = WaitForSingleObject(hThread, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for thread failed");
	//��������� ���������� ������ thread
	if(!CloseHandle(hThread))
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