//C++ 11

#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;

#define SYNC

const size_t P = 3;	//���������� �������

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"     //���������������� ���������� �������� �������������
using namespace sync_objs;
barrier_thread* br;
#endif

DWORD tm;

//������� ��������� ��������� ����� �����
DWORD rndtm(void)
{
	return rand() % (2 * P) * 500 + 1000;
}

DWORD WINAPI thread(LPVOID pnum)
{
	DWORD tmt;
	const char *vr = " ";
	if(*static_cast<size_t*>(pnum) < 10)
		vr = "  ";
	//������������� ���������� ��������� �����
	srand(tm * (*static_cast<size_t*>(pnum) + 1));
	//����� ������
	Sleep(rndtm());
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " enter in barrier: " <<  GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(*((int*)pnum));//�������������
#endif
	tmt = GetTickCount() - tm;
	Sleep(30 * *static_cast<size_t*>(pnum));
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " leave barrier:    " <<  tmt  << " ms"  << endl;
	Sleep(rndtm());
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(*static_cast<size_t*>(pnum));//�������������
#endif
	tmt = GetTickCount() - tm;
	Sleep(30 * *static_cast<size_t*>(pnum));
	cout << "thread" << vr << *static_cast<size_t*>(pnum) << " leave barrier:    " << tmt << " ms"  << endl;
	return 0;
}

int main(int argc, char** argv)
{
	//����� ������� ��������
	tm = GetTickCount();
	//������������� ���������� ��������� �����
	srand(tm);
#ifdef SYNC
	br = new barrier_thread(P);//�������� ������� ��� P �������
#endif
	//�������� �������� �������
	HANDLE  hThread[P - 1];
	size_t thm[P - 1];
	for(size_t th = 0; th < P - 1; th++)
	{
		thm[th] = th + 1;
		hThread[th]=CreateThread(nullptr, 0, thread, &thm[th], 0, nullptr);
		if (hThread[0] == nullptr)
			Error("Can't create thread");
	}
	//����� ������
	Sleep(rndtm());
	cout << "main      enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(P);//�������������
#endif
	cout << "main      leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	Sleep(1000);
	cout << endl;
#ifdef SYNC
	br->reset();//����� �������
#endif	
	Sleep(rndtm());	
	cout << "main      enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
		br->wait(P);//�������������
#endif
	cout << "main      leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	Sleep(1000);
	cout << endl;
	//�������� ���������� �������� �������
	DWORD status = WaitForMultipleObjects(P - 1, hThread, true, INFINITE);
	if(status == WAIT_FAILED)
        Status(status, "Wait for threads failed");
	//�������� ������������
	for(size_t th = 0; th < P - 1; th++)
        if(!CloseHandle(hThread[th]))
        	Error("Can't close handle");
#ifdef SYNC
	delete br;//����������� �������
#endif
	//�������� ����� ������� �� ���������� ��������
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
