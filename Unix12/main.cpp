//g++ -c main.cpp
//g++ main.o -L../sync_objs -L../error -lsync_objs -lerror -pthread -o main
//LD_LIBRARY_PATH="полный путь до sync_objs.so;полный путь до error.so":${LD_LIBRARY_PATH}
//export LD_LIBRARY_PATH

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "../error/error.hpp"


using std::cout;
using std::cin;
using std::endl;

const pid_t ERROR = -1;

const size_t P = 3; //Количество процессов

#define SYNC

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"		//Пользовательская библиотека объектов синхронизации
using namespace sync_objs;
#endif

//Функция генерации случайных целых чисел
uint32_t rndtm(void)
{
	return (rand() % (2 * P) * 500 + 1000) * 1000;
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

int main(int argc, char** argv)
{
    //Время запуска процесса
	uint32_t tm = GetTickCount();
#ifdef SYNC
	barrier_process* br = new barrier_process(P);//Создание барьера для P процессов
#endif
    for(size_t k = 1; k < P ; k++)
        switch(fork())
        {
            case ERROR:
                    Error("The new process is not created");
            case 0:
                    //Потомок
                    const char *vr;
                    if(k < 10)
                        vr = "  ";
                    else
                        vr = " ";
                    //Инициализация генератора случайных чисел 
                    srand(GetTickCount()* k);
                    //Вывод данных
                    usleep(rndtm());
                    cout << "child" << vr << k << " enter in barrier: " <<  GetTickCount() - tm << " ms"  << endl;
                #ifdef SYNC
                    br->wait(k);//Синхронизация
                #endif
                    uint32_t tmt = GetTickCount() - tm;
                    usleep(30000 * k);
                    cout << "child" << vr << k << " leave barrier:    " << tmt << " ms"  << endl;
                    usleep(rndtm());
                    cout << "child" << vr << k << " enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
                #ifdef SYNC
                    br->wait(k);//Синхронизация
                #endif
                    tmt = GetTickCount() - tm;
                    usleep(30000 * k);
                    cout << "child" << vr << k << " leave barrier:    " << tmt << " ms"  << endl;
                #ifdef SYNC
                    delete br;//Уничтожение барьера 
                #endif
                    return 0;
        }
    //Родитель
    //Инициализация генератора случайных чисел 
	srand(GetTickCount());
    //Вывод данных
	usleep(rndtm());
	cout << "main     enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(0);//Синхронизация
#endif
	cout << "main     leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	usleep(1000000);
	cout << endl;
#ifdef SYNC
	br->reset();//Сброс барьера
#endif	
	usleep(rndtm());	
	cout << "main     enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
		br->wait(0);//Синхронизация
#endif
	cout << "main     leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	usleep(1000000);
	cout << endl;
    //Ждем, пока все дочерние процессы закончат работу
    for(size_t k = 1; k < P; k++)
        if(wait(nullptr) == ERROR)
            Error("Wait for process failed");
#ifdef SYNC
	delete br;//Уничтожение барьера
#endif
    //Ожидание ввода команды на завершение процесса
    cout << "Press enter to exit." << endl;
    cin.get();
    return 0;
}