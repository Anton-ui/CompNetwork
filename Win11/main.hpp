#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::flush;

const size_t P = 3;

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
#endif

//Функция генерации случайных целых чисел
DWORD rndtm(void)
{
	return rand() % 10;
}
