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

const size_t P = 4;//Количество потоков

#define SYNC

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
critical_section_n_thread* csn;
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

void* thread(void* pnum)
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
			usleep(rndtm());
		}
		cout << "thread" << num << endl;
#ifdef SYNC
		csn->leave_region(num);//Выход из критической секции
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
	csn = new critical_section_n_thread(P);//создание критической секции для P потоков
#endif
	//Создаем потоки
    pthread_t   hThread[P - 1];
    int arg[P - 1];
	int status;
	for(size_t i = 0; i < P - 1; i++)
    {
        arg[i] = i + 1;
        if(status = pthread_create(&hThread[i], nullptr, thread, arg + i))
            Status(status, "Can't create thread");
    }
	//Вывод данных
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		csn->enter_region(0);//Вход в критическую секцию
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' '  << flush;
			usleep(rndtm());
		}
		cout << "main" << endl;
#ifdef SYNC
		csn->leave_region(0);//Выход из критической секции
#endif
        usleep(rndtm() * N2);
	}
	//Ждем завершения потоков
	for(size_t i = 0; i < P - 1; i++)
        if(status = pthread_join(hThread[i], nullptr))
            Status(status, "Can't join thread");
#ifdef SYNC
	delete csn;//Уничтожение критической секции
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
