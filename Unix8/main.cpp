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

#define SYNC

const size_t P = 3;	//Количество потоков

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"     //Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
barrier_thread* br;
#endif

uint32_t tm;

uint32_t GetTickCount() 
{
    struct timespec ts;
    unsigned theTick;
    clock_gettime(CLOCK_REALTIME, &ts);
    theTick  = ts.tv_nsec / 1000000;
    theTick += ts.tv_sec * 1000;
    return theTick;
}

//Функция генерации случайных целых чисел
uint32_t rndtm(void)
{
	return (rand() % (2 * P) * 500 + 1000) * 1000;
}

void* thread(void* pnum)
{
	uint32_t tmt;
	const char *vr = " ";
	if(*static_cast<size_t*>(pnum) < 10)
		vr = "  ";
	//Инициализация генератора случайных чисел
	srand(tm * (*static_cast<size_t*>(pnum) + 1));
	//Вывод данных
	usleep(rndtm());
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " enter in barrier: " <<  GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(*((int*)pnum));//Синхронизация
#endif
	tmt = GetTickCount() - tm;
	usleep(30000 * *static_cast<size_t*>(pnum));
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " leave barrier:    " <<  tmt  << " ms"  << endl;
	usleep(rndtm());
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(*static_cast<size_t*>(pnum));//Синхронизация
#endif
	tmt = GetTickCount() - tm;
	usleep(30000 * *static_cast<size_t*>(pnum));
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " leave barrier:    " << tmt << " ms"  << endl;
	return 0;
}

int main(int argc, char** argv)
{
	//Время запуска процесса
	tm = GetTickCount();
	//Инициализация генератора случайных чисел
	srand(tm);
#ifdef SYNC
	br = new barrier_thread(P);//Создание барьера для P потоков
#endif
	//Создание дочерних потоков
	pthread_t   hThread[P - 1];
    size_t arg[P - 1];
	int status;
	for(size_t i = 0; i < P - 1; i++)
    {
        arg[i] = i + 1;
        if(status = pthread_create(&hThread[i], nullptr, thread, arg + i))
            Status(status, "Can't create thread");
    }
	//Вывод данных
	usleep(rndtm());
	cout << "main      enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(0);//Синхронизация
#endif
	cout << "main      leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	usleep(1000000);
	cout << endl;
#ifdef SYNC
	br->reset();//Сброс барьера
#endif	
	usleep(rndtm());	
	cout << "main      enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
		br->wait(0);//Синхронизация
#endif
	cout << "main      leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	usleep(1000000);
	cout << endl;
	//Ожидание завершения дочерних потоков
	for(size_t i = 0; i < P - 1; i++)
        if(status = pthread_join(hThread[i], nullptr))
            Status(status, "Can't join thread");
#ifdef SYNC
	delete br;//Уничтожение барьера
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
