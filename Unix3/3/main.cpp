//g++ main.cpp -o main -pthread -std=c++11

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Status(int, const char*);
void Error(const char*);

const useconds_t S1 = 1500000;//мкс
const useconds_t S2 = 1000000;//мкс

const size_t N = 10;

volatile size_t a[N];

sem_t sem;

void* thread(void*)
{
	//Заполнение массива данными
	for(size_t i = 0; i < N; i++)
	{
		a[i] = i + 1;
		//Отмечаем, что один элемент готов
        if(sem_post(&sem))
            Error("Can't release semaphore");
        //
        usleep(S1);
	}
    return 0;
}

int main(int argc, char** argv)
{
	//Создаем семафор
    if(sem_init(&sem, false, 0))
        Error("Can't create semaphore");
    //Вывод начального состояния массива
	cout << "Initial state of the array:  ";
	for(size_t i = 0; i < N; i++)
		cout << a[i] << ' ' << flush;
	cout << endl;
    //Создаем поток
	pthread_t  hThread;
	int status;
	if(status = pthread_create(&hThread, nullptr, thread, nullptr)) 
        Status(status, "Can't create thread");
	//Вывод модмфицированного состояния массива
    usleep(S2);
    cout << "Modified state of the array: ";
	for(size_t i = 0; i < N; i++)
	{
		//Ожидание готовности данных
        if(sem_wait(&sem))
            Error("Wait for semaphore failed");
        //
        cout << a[i] << ' ' << flush;
		usleep(S2);
	}
	cout << endl;
	//Ждем, пока поток thread закончит свою работу
    if(status = pthread_join(hThread, nullptr)) 
        Status(status, "Can't join thread");
	//Вывод окончательного состояния массива
    cout << "Final state of the array:    ";
	for(size_t i = 0; i < N; i++)
		cout << a[i] << ' ' << flush;
	cout << endl;
    //Закрываем семафор
    if(sem_destroy(&sem))
        Error("Close semaphore failed");
    //Ждем команду на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}

void Status(int status, const char* str)
{
	cout << str << ", status = 0x" << hex << status 
        << '(' << dec << status << ')'<< endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(status);
}

void Error(const char* str)
{
    cout << str << ", errno = 0x" << hex << errno
        << '(' << dec << errno << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(errno);
}