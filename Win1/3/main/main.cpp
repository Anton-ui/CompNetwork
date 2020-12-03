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

const DWORD S = 30;//мс

int main(int argc, char** argv)
{
	//Cоздаем новый консольный процесс
	char lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\console\\console.exe", _MAX_PATH);
	PROCESS_INFORMATION  pi;
	STARTUPINFO  si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if(!CreateProcess(lpszAppName, nullptr, nullptr, nullptr, false,
			0, nullptr, nullptr, &si, &pi))
        Error("The new process is not created");
	//Выводим данные
	for(size_t j = 0; j < 10; j++)
	{
		//Выводим строку чисел j
        for(size_t i = 0; i < 10; i++)
		{
			cout << 0 << j << ' ' << flush;
			Sleep(S);
		}
		cout << "parent" << endl;
		Sleep(S*10);
	}
	//Ждем, пока дочерний процесс закончит работу
	DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for process failed");
	//Закрываем дескрипторы дочернего процесса в текущем процессе
	if(!CloseHandle(pi.hThread))
		Error("Can't close handle");
	if(!CloseHandle(pi.hProcess))
		Error("Can't close handle");
	//Ждем команду на завершение процесса
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