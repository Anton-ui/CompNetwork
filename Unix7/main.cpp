//g++ -c main.cpp
//g++ main.o -L../sync_objs -L../error -lsync_objs -lerror -pthread -o main
//LD_LIBRARY_PATH="/home/anton/���������/Prog2/sync_objs;/home/anton/���������/Prog2/error"
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

const size_t P = 4;//���������� �������

#define SYNC

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"		//���������������� ���������� �������� �������������
using namespace sync_objs;
critical_section_n_thread* csn;
#endif

const useconds_t N1 = 10;
const useconds_t N2 = 50;

//������� ��������� ��������� ��������� ����������� 0 - 10�c
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
	//������������� ���������� ��������� �����
	srand(GetTickCount()*(num + 1));
	//����� ������
	for (size_t j = 0; j < 10; ++j)
	{
		
#ifdef SYNC
		csn->enter_region(num);//���� � ����������� ������
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			usleep(rndtm());
		}
		cout << "thread" << num << endl;
#ifdef SYNC
		csn->leave_region(num);//����� �� ����������� ������
#endif
        usleep(rndtm() * N1);
	}
	
  return 0;
}

int main(int argc, char** argv)
{
	//������������� ���������� ��������� �����
	srand(GetTickCount());
#ifdef SYNC
	csn = new critical_section_n_thread(P);//�������� ����������� ������ ��� P �������
#endif
	//������� ������
    pthread_t   hThread[P - 1];
    int arg[P - 1];
	int status;
	for(size_t i = 0; i < P - 1; i++)
    {
        arg[i] = i + 1;
        if(status = pthread_create(&hThread[i], nullptr, thread, arg + i))
            Status(status, "Can't create thread");
    }
	//����� ������
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		csn->enter_region(0);//���� � ����������� ������
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' '  << flush;
			usleep(rndtm());
		}
		cout << "main" << endl;
#ifdef SYNC
		csn->leave_region(0);//����� �� ����������� ������
#endif
        usleep(rndtm() * N2);
	}
	//���� ���������� �������
	for(size_t i = 0; i < P - 1; i++)
        if(status = pthread_join(hThread[i], nullptr))
            Status(status, "Can't join thread");
#ifdef SYNC
	delete csn;//����������� ����������� ������
#endif
	//�������� ����� ������� �� ���������� ��������
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
