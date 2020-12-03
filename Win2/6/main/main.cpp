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

const DWORD S = 30;//��
const size_t N = 3;//�� ������ MAXIMUM_WAIT_OBJECTS

int main(int argc, char** argv)
{
	DWORD status;
    //������� �������
	HANDLE hMutex = CreateMutex(nullptr, false, "DemoMutex");
	if(hMutex == nullptr)
        Error("Create mutex failed");
    //C������ ����� ���������� ��������
	char lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\console\\console.exe", _MAX_PATH);
	STARTUPINFO  si[N];
	PROCESS_INFORMATION  pi[N];
	HANDLE hProcess[N];
	char (*args)[11] = new char[N][11];
	for(size_t i = 0; i < N; i++)
	{
		sprintf(args[i], "%s %i", "console", i + 1);
		ZeroMemory(si + i, sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		if(!CreateProcess(lpszAppName, args[i], nullptr, nullptr, false,
                0, nullptr, nullptr, si + i, pi + i))
            Error("The new process is not created");
        hProcess[i] = pi[i].hProcess;
	}
	//������� �� ����� ������
	for(size_t j = 0; j < 10; j++)
	{
		//����������� �������
		status = WaitForSingleObject(hMutex, INFINITE);
		if(status != WAIT_OBJECT_0)
    		Status(status, "Wait for mutex failed");
        //������� ������ ����� j
        for(size_t i = 0; i < 10; i++)
		{
			cout << 0 << j << ' ' << flush;
			Sleep(S);
		}
		cout << "parent" << endl;
        //����������� �������
		if(!ReleaseMutex(hMutex))
			Error("Can't release mutex");
        //
        Sleep(S*10);
	}
	//����, ���� ��� �������� �������� �������� ������
	status = WaitForMultipleObjects(N, hProcess, true, INFINITE);
    if(status == WAIT_FAILED)
        Status(status, "Wait for process failed");
    //��������� ����������� �������� ��������� � ������� ��������
	for(size_t i = 0; i < N; i++)
	{
		if(!CloseHandle(pi[i].hThread))
            Error("Can't close handle");
        if(!CloseHandle(pi[i].hProcess))
            Error("Can't close handle");
	}
	delete[] args;
    //��������� ���������� ��������
	if(!CloseHandle(hMutex))
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