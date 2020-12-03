//c++ 11

#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;

const size_t P = 3; //���������� �������
const size_t N = 9; //���������� ��������� �����

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//���������������� ���������� �������� �������������
using namespace sync_objs;
rotation_thread* rt;
#endif

//������� ��������� ��������� ��������� ����������� 1-5c
DWORD rndtm(void)
{
	return (rand() % 4 + 1) * 1000;
}

DWORD WINAPI thread(LPVOID pnum)
{
	//������������� ���������� ��������� �����
	srand(GetTickCount()*(*static_cast<size_t*>(pnum) + 1));
	//����� ������
	for(size_t i = 1 + *static_cast<size_t*>(pnum); i <= N; i+=P)
	{
		Sleep(rndtm());
#ifdef SYNC
		rt -> enter_region(*static_cast<size_t*>(pnum));//�������������
#endif
		cout << i << endl;
#ifdef SYNC
		rt -> leave_region();//�������������
#endif
	}
    return 0;
}

int main(int argc, char** argv)
{
	//������������� ���������� ��������� �����
	srand(GetTickCount());
#ifdef SYNC
	rt = new rotation_thread(P);//�������� ������� "����������� ��� � �������"
#endif
	//�������� �������� �������
	HANDLE  hThread[P - 1];
	size_t thm[P - 1];
	for(size_t th = 0; th < P - 1; th++)
	{
		thm[th] = th + 1;
		hThread[th] = CreateThread(nullptr, 0, thread, &thm[th], 0, nullptr);
		if(hThread[th] == nullptr)
			Error("Can't create thread");
	}
	//����� ������
	for(size_t i = 1; i <= N; i+=P)
	{
		Sleep(rndtm());
#ifdef SYNC
		rt -> enter_region(0);//�������������
#endif
		cout << i << endl;		
#ifdef SYNC
		rt -> leave_region();//�������������
#endif
	}
	//�������� ���������� �������� �������
    DWORD status = WaitForMultipleObjects(P - 1, hThread, true, INFINITE);
	if(status == WAIT_FAILED)
        Status(status, "Wait for threads failed");
	//�������� ������������
	for(size_t th = 0; th < P - 1; th++)
        if(!CloseHandle(hThread[th]))
        	Error("Can't close handle");
#ifdef SYNC
	delete rt;//����������� ������� "�����������"
#endif
	//�������� ����� ������� �� ���������� ��������
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
