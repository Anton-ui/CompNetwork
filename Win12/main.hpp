#include <windows.h>
#include <iostream>
#include "..\..\error\error.hpp"

using std::cout;
using std::cin;
using std::endl;

#define SYNC

const size_t P = 3; //���������� ���������

#ifdef SYNC
#include "..\..\sync_objs\sync_objs.hpp"     //���������������� ���������� �������� �������������
using namespace sync_objs;
#endif 

//������� ��������� ��������� ����� �����
DWORD rndtm()
{
	return rand() % (2 * P) * 500 + 1000;
}
