//g++ main.cpp -o main -pthread -std=c++11

#include <pthread.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Status(int, const char*);

const useconds_t S1 = 10000;//мкс
const useconds_t S2 = 30000;//мкс

void* thread(void*)
{
	//Выводим данные
	for(size_t j = 10; j < 20; ++j)
	{
		//Выводим строку чисел j
		for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			usleep(S1);
		}
		cout << "thread" << endl;
		usleep(S1*10);
	}
    return 0;
}

int main(int argc, char** argv)
{
	//Создаем поток
	pthread_t  hThread;
	int status;
	if(status = pthread_create(&hThread, nullptr, thread, nullptr)) 
        Status(status, "Can't create thread");
	//Выводим данные
	for(size_t j = 0; j < 10; ++j)
	{
		//Выводим строку чисел j
        for(size_t i = 0; i < 10; ++i)
		{
			cout << 0 << j << ' ' << flush;
			usleep(S2);
		}
		cout << " main" << endl;
		usleep(S2*10);
	}
	//Ждем, пока поток thread закончит свою работу
    if(status = pthread_join(hThread, nullptr)) 
        Status(status, "Can't join thread");
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