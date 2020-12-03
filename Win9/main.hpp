#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;

const size_t N = 9; //Количество выводимых чисел
const size_t P = 3; //Количество процессов

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
#endif

//Функция генерации случайных целых чисел
DWORD rndtm(void)
{
	return (rand() % 5 + 1) * 1000;
}
