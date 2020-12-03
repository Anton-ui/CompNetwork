#include <windows.h>
#include "sync_objs.hpp"
#include "..\error\error.hpp"

using namespace sync_objs;

static const DWORD S = 10;//��

//rotation
void rotation::enter_region(size_t process)
{ 
	while(*turn != process) Sleep(S);
} 
void rotation::leave_region() 		 
{
	if (++*turn >= *count)
		*turn = 0;
}

//rotation_thread
rotation_thread::~rotation_thread() 		 
{ 
	delete turn;
	delete count;
}
rotation_thread::rotation_thread(size_t Count) 
{
	turn = new size_t();
	*turn = 0;
	count = new size_t();
	*count = Count; 
}

// Peterson's algorithm for 2 threads
critical_section_2_thread::~critical_section_2_thread() 		 
{ 
	delete turn;
	delete interested;
}
critical_section_2_thread::critical_section_2_thread() 
{ 
	interested = new int [2];	
	turn = new int();	
	*turn = 0;
}
void critical_section_2_thread::enter_region(int process) /* process ����� �������� 0 ��� 1 */ 
{ 
	int other; 				                              /*����� ������� ��������*/
	other = 1 - process; 			                      /*�������������� �������*/
	interested[process] = TRUE;		                      /* ������������ ������������������ */ 
	*turn = process;			                              /* ��������� ������ */ 
	while (*turn == process && interested[other] == TRUE); /* ���� ��� ���������� */ 
}
void critical_section_2_thread::leave_region(int process) /* �������, ���������� ����������� ������� */ 
{ 
	interested[process] = FALSE; 	                      /* ������� ������ �� ����������� ������� */ 
}

// Peterson's algorithm for N threads
critical_section_n_thread::~critical_section_n_thread() 		 
{
	delete amount;
	delete stage;
	delete Turn;
}
critical_section_n_thread::critical_section_n_thread(size_t Count) 
{
	amount = new size_t();
	*amount = Count;
	stage = new int [Count+1];
	Turn = new int [Count];	
	for (int i = 0; i < Count; i++)
	{
		stage[i] = -1;
		Turn[i] = 0;
	}
	stage[Count] = -1;
}
void critical_section_n_thread::enter_region(int process)
{  
	for (int count = 0; count < (*amount-1); count++) 
	{
	   stage[process] = count;
	   Turn[count] = process;
	   for (int k = 1; k <= *amount; k++)
	   {
			if (k == process)
	        	continue;
	    	while (stage[k] >= count  && Turn[count] == process);
	    }
	}
}
void critical_section_n_thread::leave_region(int process) 
{ 
	stage[process] = -1;
}

// barrier
void barrier::wait(size_t process)
{
	++*ibarrier;
	while(*ibarrier < *count) Sleep(S);
}
void barrier::reset()
{
	*ibarrier = 0;
}

//barrier_thread
barrier_thread::~barrier_thread() 		 
{ 
	delete ibarrier;
	delete count;
}
barrier_thread::barrier_thread(size_t Count) 
{
	ibarrier = new size_t(0);
	*ibarrier = 0;
	count = new size_t();
	*count = Count;
}


//----------------processes---------------------

//rotation_process
rotation_process::rotation_process(const char * name, size_t Count) 		 
{ 
	if(Count != 2)
		Error("Size is out of range");
	//��������� ������ ����������� ����� � ������
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(size_t), name);
	if(hMapping == nullptr)
		Error("Create file mapping failed");
    //������� ��� �����
	turn = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(turn == nullptr)
		Error("Create view failed");
	*turn = 0;
}
rotation_process::rotation_process(const char * name) 		 
{ 
	//��������� ������ ����������� ����� � ������
	hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,name);
	if(hMapping == nullptr)
		Error("Open file mapping failed");
	//������� ��� �����
	turn = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(turn == nullptr)
		Error("Create view failed");
} 
rotation_process::~rotation_process() 		/* process ����� �������� 0 ��� 1 */ 
{ 
	//�������� ����������� ����� � ������
	if(!UnmapViewOfFile(const_cast<size_t*>(turn)))
		Error("Unmap view failed");
	//��������� ����������
	if(!CloseHandle(hMapping))
		Error("Can't close handle");
} 
