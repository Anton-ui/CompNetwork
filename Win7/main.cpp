//c++ 11

#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::flush;

const size_t P = 4;//Количество потоков

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
critical_section_n_thread* csn;
#endif

const DWORD N1 = 10;
const DWORD N2 = 50;

//Функция генерации случайных временных промежутков 0 - 10мc
DWORD rndtm(void)
{
	return rand() % 11;
}

DWORD WINAPI thread(LPVOID pnum)
{
	size_t num = *static_cast<size_t*>(pnum);
	//Инициализация генератора случайных чисел
	srand(GetTickCount()*(num + 1));
	//Вывод данных
	for (size_t j = 0; j < 10; ++j)
	{
		
#ifdef SYNC
		csn->enter_region(num);//Вход в критическую секцию
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << "thread" << num << endl;
#ifdef SYNC
		csn->leave_region(num);//Выход из критической секции
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
	csn = new critical_section_n_thread(P);//создание критической секции для P потоков
#endif
	//Создание дочерних потоков
	HANDLE  hThread[P - 1];
	size_t thm[P - 1];
	for(size_t th = 0; th < P - 1; th++)
	{
		thm[th] = th + 1;
		hThread[th]=CreateThread(nullptr, 0, thread, &thm[th], 0, nullptr);
		if (hThread[0] == nullptr)
			Error("Can't create thread");
	}
	//Вывод данных
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		csn->enter_region(P);//Вход в критическую секцию
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' '  << flush;
			Sleep(rndtm());
		}
		cout << "main" << endl;
#ifdef SYNC
		csn->leave_region(P);//Выход из критической секции
#endif
        Sleep(rndtm() * N2);
	}
	//Ожидание завершения дочерних потоков
    DWORD status = WaitForMultipleObjects(P - 1, hThread, true, INFINITE);
	if(status == WAIT_FAILED)
        Status(status, "Wait for threads failed");
	//Закрытие дескрипторов
	for(size_t th = 0; th < P - 1; th++)
        if(!CloseHandle(hThread[th]))
        	Error("Can't close handle");
#ifdef SYNC
	delete csn;//Уничтожение критической секции
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
