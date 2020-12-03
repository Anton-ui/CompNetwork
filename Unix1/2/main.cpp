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
const size_t N = 3;

void* thread(void* arg)
{
	//Выводим данные
	for(size_t j = *static_cast<size_t*>(arg) * 10, max = j + 10; j < max; j++)
	{
		//Выводим строку чисел j
		for(size_t i = 0; i < 10; i++)
		{
			cout << j << ' ' << flush;
			usleep(S1);
		}
		cout << "thread" << *static_cast<size_t*>(arg) << endl;
		usleep(S1*10);
	}
	return 0;
}

int main(int argc, char** argv)
{
	int status;
    //Создаем потоки
    pthread_t   hThread[N];
    size_t arg[N];
	for(size_t i = 0; i < N; i++)
    {
        arg[i] = i + 1;
        if(status = pthread_create(hThread + i, nullptr, thread, arg + i))
            Status(status, "Can't create thread");
    }
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
    //Ждем завершения потоков
	for(size_t i = 0; i < N; i++)
        if(status = pthread_join(hThread[i], nullptr))
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