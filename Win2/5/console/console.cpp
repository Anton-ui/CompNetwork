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

const DWORD S = 10;//мс

int main(int argc, char** argv)
{
	//Открываем мьютекс
	HANDLE hMutex = OpenMutex(SYNCHRONIZE, false, "DemoMutex");
    if(hMutex == nullptr)
        Error("Open mutex failed");
    //Выводим данные
	for(size_t j = 10; j < 20; ++j)
	{
		//Захватываем мьютекс
		DWORD status = WaitForSingleObject(hMutex, INFINITE);
		if(status != WAIT_OBJECT_0)
    		Status(status, "Wait for mutex failed");
        //Выводим строку чисел j
        for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(S);
		}
		cout << "child" << endl;
        //Освобождаем мьютекс
		if(!ReleaseMutex(hMutex))
			Error("Can't release mutex");
        //
        Sleep(S*10);
  	} 
    //Закрываем дескриптор мьютекса
	if(!CloseHandle(hMutex))
        Error("Can't close handle");
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