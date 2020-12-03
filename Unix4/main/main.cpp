//g++ -c main.cpp
//g++ main.o -L../sync_objs -L../error -lsync_objs -lerror -pthread -o main
//LD_LIBRARY_PATH="полный путь до sync_objs.so;полный путь до error.so":${LD_LIBRARY_PATH}
//export LD_LIBRARY_PATH


#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include "../error/error.hpp"

#define SYNC

using std::cout;
using std::cin;
using std::endl;

const size_t P = 2; //Количество потоков
const size_t N = 9; //Количество выводимых чисел

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
rotation_thread* rt;
#endif

//Функция генерации случайных временных промежутков 1-5c
useconds_t rndtm(void)
{
	return (rand() % 4 + 1) * 1000000;
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
	//Инициализация генератора случайных чисел
	srand(GetTickCount()*(*(size_t*)pnum + 1));
	//Вывод данных
	for (size_t i = 1 + *(size_t*)pnum; i <= N; i+=P)
	{
		usleep(rndtm());
#ifdef SYNC
		rt -> enter_region(*(size_t*)pnum);//Синхронизация
#endif
		cout << i << endl;
#ifdef SYNC
		rt -> leave_region();//Синхронизация
#endif
	}
    return 0;
}

int main(int argc, char** argv)
{
	//Инициализация генератора случайных чисел
	srand(GetTickCount());
#ifdef SYNC
	rt = new rotation_thread(P);//Создание объекта "чередование для Р потоков"
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
	for (size_t i = 1; i <= N; i+=P)
	{
		usleep(rndtm());
#ifdef SYNC
		rt -> enter_region(0);//Синхронизация
#endif
		cout << i << endl;		
#ifdef SYNC
		rt -> leave_region();//Синхронизация
#endif
	}
	//Ждем завершения потоков
	for(size_t i = 0; i < P - 1; i++)
        if(status = pthread_join(hThread[i], nullptr))
            Status(status, "Can't join thread");
#ifdef SYNC
	delete rt;//Уничтожение объекта "чередование"
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}