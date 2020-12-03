//g++ main.cpp -o main -std=c++11

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Error(const char*);

const pid_t ERROR = -1;

const useconds_t S1 = 1500000;//мкс
const useconds_t S2 = 1000000;//мкс

const size_t N = 10;   // размер массива

int main(int argc, char** argv)
{
    //Создаем объект отображения
    volatile size_t *ptr;
    if((ptr = static_cast<volatile size_t*>(mmap(nullptr, N * sizeof(size_t), PROT_READ | PROT_WRITE,
            MAP_ANON | MAP_SHARED, -1, 0))) == MAP_FAILED)
        Error("Create file mapping failed");
    //Инициализируем массив 
	cout << "Initial state of the array:  ";
    for(size_t i = 0; i < N; i++)
		cout << (ptr[i] = 0) << ' ' << flush;
	cout << endl;
    //Создаем дочерний процесс
    switch(fork())
    {
        case -1:
                Error("The new process is not created");
        case 0:
                //Потомок
                //Заполняем массив
                for(size_t i = 0; i < N; i++)
                {
                    ptr[i] = i + 1;
                    usleep(S1);
                }
                //Удаляем объект отображения
                if(munmap(const_cast<size_t*>(ptr), N * sizeof(size_t)))
                    Error("Unmap view failed");
                return 0;
        default:
                //Родитель
                //Выводим массив в консоль
                usleep(S2);
                cout << "Modified state of the array: ";
                for(size_t i = 0; i < N; i++)
                {
                    cout << ptr[i] << ' ' << flush;
                    usleep(S2);
                }
                cout << endl;
                //Ждем завершения созданного прцесса
                if(wait(nullptr) == ERROR)
                    Error("Wait for process failed");
                //Вывод окончательного состояния массива
                cout << "Final state of the array:    ";
                for(size_t i = 0; i < 10; i++)
                    cout << ptr[i] << ' ' << flush;
                cout << endl;
                //Удаляем объект отображения
                if(munmap(const_cast<size_t*>(ptr), N * sizeof(size_t)))
                    Error("Unmap view failed");
                //Ждем команду на завершение процесса
                cout << "Press enter to exit." << endl;
                cin.get();
                return 0;
    }
}

void Error(const char* str)
{
    cout << str << ", errno = 0x" << hex << errno
        << '(' << dec << errno << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(errno);
}