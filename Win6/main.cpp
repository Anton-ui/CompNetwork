//c++ 11

#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::flush;

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
critical_section_2_thread* cs2;
#endif

const DWORD N1 = 10;
const DWORD N2 = 50;

//Функция генерации случайных временных промежутков 0 - 10мc
DWORD rndtm(void)
{
	return rand() % 11;
}

DWORD WINAPI thread(LPVOID)
{
	//Инициализация генератора случайных чисел
	srand(GetTickCount()*2);
	//Вывод данных
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		cs2->enter_region(0);//Вход в критическую секцию
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << "thread" << endl;
#ifdef SYNC
		cs2->leave_region(0);//Выход из критической секции
#endif
		Sleep(rndtm() * N1);
	}
    return 0;
}

int main(int argc, char** argv)
{
	//Инициализация генератора случайных чисел
	srand(GetTickCount());
#ifdef SYNC
	cs2 = new critical_section_2_thread(); //создание критической секции для 2 потоков
#endif
	//Создание дочерних потоков
	HANDLE hThread=CreateThread(nullptr, 0, thread, NULL, 0, NULL);
	if (hThread == nullptr)
		Error("Can't create thread");	
	//Вывод данных
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		cs2->enter_region(1);//Вход в критическую секцию
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << "main" << endl;		
#ifdef SYNC
		cs2->leave_region(1);//Выход из критической секции
#endif
		Sleep(rndtm() * N2);
	}
	//Ожидание завершения дочерних потоков
    DWORD status = WaitForSingleObject(hThread, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for threads failed");
	//Закрытие дескрипторов
    if(!CloseHandle(hThread))
    	Error("Can't close handle");
#ifdef SYNC
	delete cs2;//Уничтожение критической секции
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
