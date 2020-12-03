//c++ 11

#include <windows.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;

void Error(const char*);

const size_t N = 10;// размер массива
const DWORD S = 1500;//мс

int main(int argc, char** argv)
{
	//Открываем семафор    
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_MODIFY_STATE, FALSE, "DemoSemaphore");
	if(hSemaphore == nullptr)
		Error("Can't create semaphore");
	//Открываем объект отображения файла в память
	HANDLE hMapping = OpenFileMapping(FILE_MAP_WRITE, false, "MappingName");
	if(hMapping == nullptr)
		Error("Open file mapping failed");
	//Создаем вид файла
	volatile size_t *ptr = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0));
    if(ptr == nullptr)
		Error("Create view failed");
	//Заполняем массив
	for(size_t i = 0; i < N; i++)
	{
		ptr[i] = i + 1;
		//Отмечаем, что один элемент готов
		if(!ReleaseSemaphore(hSemaphore,1,nullptr))
            Error("Can't release semaphore");
        //
		Sleep(S);
	}
	//Отменяем отображение файла в память
	if(!UnmapViewOfFile(const_cast<size_t*>(ptr)))
        Error("Unmap view failed");
	//Закрываем дескрипторы
	if(!CloseHandle(hMapping))
        Error("Can't close handle");
	if(!CloseHandle(hSemaphore))
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