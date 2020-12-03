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

const size_t N = 10;// размер массива
const DWORD S = 1000;//мс

int main(int argc, char** argv)
{
	DWORD status;
	//Открываем объект отображения файла в память
	HANDLE hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
        PAGE_READWRITE, 0, N * sizeof(size_t), "MappingName");
	if(hMapping == nullptr)
		Error("Create file mapping failed");
	//Создаем вид файла
	volatile size_t *ptr = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0));
    if(ptr == nullptr)
		Error("Create view failed");
    //Инициализируем массив 
	cout << "Initial state of the array:  ";
    for(size_t i = 0; i < N; ++i)
		cout << (ptr[i] = 0) << ' ' << flush;
	cout << endl;
	//Создаем семафор    
	HANDLE hSemaphore = CreateSemaphore(nullptr, 0, N, "DemoSemaphore");
	if(hSemaphore == nullptr)
		Error("Can't create semaphore");
	//Создаем процесс, который будет читать данные из отображаемого в память файла
	char  lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\console\\console.exe", _MAX_PATH );
    PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if(!CreateProcess(lpszAppName, nullptr, nullptr, nullptr, false,
            0, nullptr, nullptr, &si, &pi))
	    Error("The new process is not created"); 
	//Выводим массив на консоль
    Sleep(S);
	cout << "Modified state of the array: ";
    for(size_t i = 0; i < N; i++)
	{
		//Ожидание готовности данных
        status = WaitForSingleObject(hSemaphore, INFINITE);
        if(status != WAIT_OBJECT_0)
    		Status(status, "Wait for semaphore failed");
        //
		cout << ptr[i] << ' ' << flush;
        Sleep(S);
	}
	cout << endl;
    //Ждем завершения созданного прцесса
	status = WaitForSingleObject(pi.hProcess, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for process failed");
	//Вывод окончательного состояния массива
    cout << "Final state of the array:    ";
    for(size_t i = 0; i < N; i++)
		cout << ptr[i] << ' ' << flush;
	cout << endl;
	//Отменяем отображение файла в память
	if(!UnmapViewOfFile(const_cast<size_t*>(ptr)))
        Error("Unmap view failed");
	//Закрываем дескрипторы
	if(!CloseHandle(pi.hThread))
		Error("Can't close handle");
	if(!CloseHandle(pi.hProcess))
		Error("Can't close handle");
	if(!CloseHandle(hMapping))
        Error("Can't close handle");
    if(!CloseHandle(hSemaphore))
        Error("Can't close handle");
    //Ждем команду на завершение процесса
	cout << "Press enter to exit.";
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