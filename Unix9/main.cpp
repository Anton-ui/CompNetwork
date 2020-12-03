//g++ -c main.cpp
//g++ main.o -L../sync_objs -L../error -lsync_objs -lerror -pthread -o main
//LD_LIBRARY_PATH="������ ���� �� sync_objs.so;������ ���� �� error.so":${LD_LIBRARY_PATH}
//export LD_LIBRARY_PATH

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "../error/error.hpp"


using std::cout;
using std::cin;
using std::endl;

const pid_t ERROR = -1;

const size_t N = 9; //���������� ��������� �����
const size_t P = 3; //���������� ���������

#define SYNC

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"		//���������������� ���������� �������� �������������
using namespace sync_objs;
#endif

//������� ��������� ��������� ����� �����
uint32_t rndtm(void)
{
	return (rand() % 5 + 1) * 1000000;
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
#ifdef SYNC
	rotation_process* rt = new rotation_process(P);//�������� ������� "����������� ��� � ���������"
#endif
    for(size_t k = 1; k < P ; k++)
        switch(fork())
        {
            case ERROR:
                     Error("The new process is not created");
            case 0:
                    //�������
                    //������������� ���������� ��������� ����� 
                    srand(GetTickCount() * (k + 1));
                    //����� ������
                    for (size_t i = 1 + k; i <= N; i+=P)
                    {
                        usleep(rndtm());
                #ifdef SYNC
                        rt -> enter_region(k);//�������������
                #endif
                        cout << i << endl;
                #ifdef SYNC
                        rt -> leave_region();//�������������
                #endif
                        }
                #ifdef SYNC
                    delete rt;//���������� �� ������� "�����������"
                #endif
                    return 0;
        }
    //��������
    //������������� ���������� ��������� ����� 
	srand(GetTickCount());
    //����� ������
	for (size_t i = 1; i <= N; i+=P)
	{
		usleep(rndtm());
#ifdef SYNC
		rt->enter_region(0);//�������������
#endif
		cout << i << endl;
#ifdef SYNC
		rt->leave_region();//�������������
#endif
	}
    //����, ���� ��� �������� �������� �������� ������
    for(size_t k = 1; k < P; k++)
        if(wait(nullptr) == ERROR)
            Error("Wait for process failed");
#ifdef SYNC
	delete rt;//����������� ������� "�����������"
#endif
    //�������� ����� ������� �� ���������� ��������
    cout << "Press enter to exit." << endl;
    cin.get();
    return 0;
}