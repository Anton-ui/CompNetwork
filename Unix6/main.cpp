//g++ -c main.cpp
//g++ main.o -L../sync_objs -L../error -lsync_objs -lerror -pthread -o main
//LD_LIBRARY_PATH="/home/anton/Документы/Prog2/sync_objs;/home/anton/Документы/Prog2/error"
//export LD_LIBRARY_PATH
//echo $LD_LIBRARY_PATH

#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include "../error/error.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::flush;

#define SYNC

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"			//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
critical_section_2_thread* cs2;
#endif

const useconds_t N1 = 10;
const useconds_t N2 = 50;

//Функция генерации случайных временных промежутков 0 - 10мc
useconds_t rndtm(void)
{
	return (rand() % 11) * 1000;
}

uint32_t GetTickCount() 
{
    struct timespec ts;
    unsigned theTick;
    clock_gettime(CLOCK_REALTIME, &ts);
    theTick  = ts.tv_nsec / 1000000;
    theTick += ts.tv_sec * 1000;
    return theTick;
}

void* thread(void*)
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
			usleep(rndtm());
		}
		cout << "thread" << endl;
#ifdef SYNC
		cs2->leave_region(0);//Выход из критической секции
#endif
		usleep(rndtm() * N1);
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
	//Создание дочернего потока
	pthread_t  hThread;
	int status;
    if(status = pthread_create(&hThread, nullptr, thread, nullptr))
            Status(status, "Can't create thread");
	//Вывод данных
	for(size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		cs2->enter_region(1);//Вход в критическую секцию
#endif
		for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			usleep(rndtm());
		}
		cout << "main" << endl;
#ifdef SYNC
		cs2->leave_region(1);//Выход из критической секции
#endif
		usleep(rndtm() * N2);
	}
	//Ожидание завершения дочернего потока
	if(status = pthread_join(hThread, nullptr))
        Status(status, "Can't join thread");   
#ifdef SYNC
	delete cs2;//Уничтожение критической секции
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}