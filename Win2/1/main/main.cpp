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

const DWORD S1 = 10;//мс
const DWORD S2 = 30;//мс

CRITICAL_SECTION cs;

DWORD WINAPI thread(LPVOID)
{
	//Выводим данные
	for(size_t j = 10; j < 20; ++j)
	{
		//Входим в критическую секцию
		EnterCriticalSection (&cs);
        //Выводим строку чисел j
		for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(S1);
		}
		cout << "thread" << endl;
        //Выходим из критической секции
		LeaveCriticalSection(&cs);
		//
        Sleep(S1*10);
	}
	return 0;
}

int main(int argc, char** argv)
{
	//Инициализируем критическую секцию
	InitializeCriticalSection(&cs);
    //Создаем поток
	HANDLE hThread = CreateThread(nullptr, 0, thread, nullptr, 0, nullptr);
	if(hThread == nullptr)
        Error("Can't create thread");
	//Выводим данные
	for(size_t j = 0; j < 10; ++j)
	{
		//Входим в критическую секцию
		EnterCriticalSection (&cs);
        //Выводим строку чисел j
        for(size_t i = 0; i < 10; ++i)
		{
			cout << 0 << j << ' ' << flush;
			Sleep(S2);
		}
		cout << " main" << endl;
        //Выходим из критической секции
		LeaveCriticalSection(&cs);
		//
        Sleep(S2*10);
	}
	//Ждем завершения потока thread
	DWORD status = WaitForSingleObject(hThread, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for thread failed");
    //Закрываем дескриптор потока thread
	if(!CloseHandle(hThread))
		Error("Can't close handle");
    //Закрываем критическую секцию
	DeleteCriticalSection(&cs);
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