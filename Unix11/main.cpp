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
using std::flush;

const pid_t ERROR = -1;

const size_t P = 3; //���������� ���������

#define SYNC

#ifdef SYNC
#include "../sync_objs/sync_objs.hpp"		//���������������� ���������� �������� �������������
using namespace sync_objs;
#endif

//������� ��������� ��������� ����� �����
uint32_t rndtm(void)
{
	return rand() % 10 * 1000;
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
	critical_section_n_process* csn = new critical_section_n_process(P); //�������� ����������� ������ ��� P ���������
#endif
    for(size_t k = 1; k < P ; k++)
        switch(fork())
        {
            case ERROR:
                    Error("The new process is not created");
            case 0:
                    //�������
                    //������������� ���������� ��������� ����� 
                    srand(GetTickCount()* k);
                    //����� ������
                    for (int j = 0; j < 10; ++j)
                    {
                        usleep(rndtm()*10);
                    #ifdef SYNC
                        csn->enter_region(k);//���� � ����������� ������
                    #endif
                        for (size_t i = 0; i < 10; ++i)
                        {
                            cout << k << j << ' ' << flush;
                            usleep(rndtm());
                        }
                        cout << " child" << k << endl;
                    #ifdef SYNC
                        //����� �� ����������� ������
                        csn->leave_region(k);
                    #endif
                    }
                #ifdef SYNC
                    delete csn;//���������� �� ����������� ������ 
                #endif
                    return 0;
        }
    //��������
    //������������� ���������� ��������� ����� 
	srand(GetTickCount());
    //����� ������
	for (size_t j = 0; j < 10; ++j)
	{
		usleep(rndtm()*10);
#ifdef SYNC
		csn->enter_region(0);//���� � ����������� ������
#endif
		for (int i = 0; i < 10; ++i)
		{
			cout << ' ' << j << ' ' << flush;
			usleep(rndtm());
		}
		cout << " parent"  << endl;
#ifdef SYNC
		csn->leave_region(0);//����� �� ����������� ������
#endif
	}
    //����, ���� ��� �������� �������� �������� ������
    for(size_t k = 1; k < P; k++)
        if(wait(nullptr) == ERROR)
            Error("Wait for process failed");
#ifdef SYNC
	delete csn;//����������� ������� "�����������"
#endif
    //�������� ����� ������� �� ���������� ��������
    cout << "Press enter to exit." << endl;
    cin.get();
    return 0;
}