#include <windows.h>
#include <iostream>
#include "..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;

const size_t N = 9; //���������� ��������� �����
const size_t P = 3; //���������� ���������

#define SYNC

#ifdef SYNC
#include "..\sync_objs\sync_objs.hpp"		//���������������� ���������� �������� �������������
using namespace sync_objs;
#endif

//������� ��������� ��������� ����� �����
DWORD rndtm(void)
{
	return (rand() % 5 + 1) * 1000;
}
