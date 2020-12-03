//c++ 11

#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::flush;

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//���������������� ���������� �������� �������������
using namespace sync_objs;
critical_section_2_thread* cs2;
#endif

const DWORD N1 = 10;
const DWORD N2 = 50;

//������� ��������� ��������� ��������� ����������� 0 - 10�c
DWORD rndtm(void)
{
	return rand() % 11;
}

DWORD WINAPI thread(LPVOID)
{
	//������������� ���������� ��������� �����
	srand(GetTickCount()*2);
	//����� ������
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		cs2->enter_region(0);//���� � ����������� ������
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << "thread" << endl;
#ifdef SYNC
		cs2->leave_region(0);//����� �� ����������� ������
#endif
		Sleep(rndtm() * N1);
	}
    return 0;
}

int main(int argc, char** argv)
{
	//������������� ���������� ��������� �����
	srand(GetTickCount());
#ifdef SYNC
	cs2 = new critical_section_2_thread(); //�������� ����������� ������ ��� 2 �������
#endif
	//�������� �������� �������
	HANDLE hThread=CreateThread(nullptr, 0, thread, NULL, 0, NULL);
	if (hThread == nullptr)
		Error("Can't create thread");	
	//����� ������
	for (size_t j = 0; j < 10; ++j)
	{
#ifdef SYNC
		cs2->enter_region(1);//���� � ����������� ������
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << "main" << endl;		
#ifdef SYNC
		cs2->leave_region(1);//����� �� ����������� ������
#endif
		Sleep(rndtm() * N2);
	}
	//�������� ���������� �������� �������
    DWORD status = WaitForSingleObject(hThread, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for threads failed");
	//�������� ������������
    if(!CloseHandle(hThread))
    	Error("Can't close handle");
#ifdef SYNC
	delete cs2;//����������� ����������� ������
#endif
	//�������� ����� ������� �� ���������� ��������
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
