#include <windows.h>
#include "sync_objs.hpp"
#include "..\error\error.hpp"

using namespace sync_objs;

static const DWORD S = 10;//мс

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
void critical_section_2_thread::enter_region(int process) /* process имеет значение 0 или 1 */ 
{ 
	int other; 				                              /*номер другого процесса*/
	other = 1 - process; 			                      /*противостоящий процесс*/
	interested[process] = TRUE;		                      /* демонстрация заинтересованности */ 
	*turn = process;			                              /* установка флажка */ 
	while (*turn == process && interested[other] == TRUE); /* цикл без инструкции */ 
}
void critical_section_2_thread::leave_region(int process) /* процесс, покидающий критическую область */ 
{ 
	interested[process] = FALSE; 	                      /* признак выхода из критической области */ 
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
	//Открываем объект отображения файла в память
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 2*sizeof(size_t), name);
	if(hMapping == nullptr)
		Error("Create file mapping failed");
    //Создаем вид файла
	turn = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(turn == nullptr)
		Error("Create view failed");
	count = turn + 1;
	*turn = 0;
	*count = Count;
}
rotation_process::rotation_process(const char * name) 		 
{ 
	//Открываем объект отображения файла в память
	hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,name);
	if(hMapping == nullptr)
		Error("Open file mapping failed");
	//Создаем вид файла
	turn = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(turn == nullptr)
		Error("Create view failed");
	count = turn + 1;
} 
rotation_process::~rotation_process() 		/* process имеет значение 0 или 1 */ 
{ 
	//Отменяем отображение файла в память
	if(!UnmapViewOfFile(const_cast<size_t*>(turn)))
		Error("Unmap view failed");
	//Закрываем дескриптор
	if(!CloseHandle(hMapping))
		Error("Can't close handle");
}

// Peterson's algorithm for N processes
critical_section_n_process::critical_section_n_process(const char * name, size_t Count)
{
	//Создаем объект отображения файла в память
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 2*sizeof(size_t)*Count, name);
	if(hMapping == nullptr)
		Error("Create file mapping failed");
    //Создаем вид файла
	count = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(count == nullptr)
		Error("Create view failed");
	turn = count + 1;
	stage = turn + Count - 1;
	//Инициализируем переменные
	*count = Count;
	for (size_t i = 0; i < *count - 1; i++)
	{
		turn[i] = 0;
		stage[i] = 0;
	}
	stage[*count - 1] = 0;
}
critical_section_n_process::critical_section_n_process(const char * name)
{
	//Открываем объект отображения файла в память
	hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,name);
	if(hMapping == nullptr)
		Error("Open file mapping failed");
	//Создаем вид файла
	count = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(count == nullptr)
		Error("Create view failed");
	turn = count + 1;
	stage = turn + *count - 1;
}
critical_section_n_process::~critical_section_n_process()
{
	//Отменяем отображение файла в память
	if(!UnmapViewOfFile(const_cast<size_t*>(count)))
		Error("Unmap view failed");
	//Закрываем дескриптор
	if(!CloseHandle(hMapping))
		Error("Can't close handle");
}
void critical_section_n_process::enter_region(size_t process)
{
	for (size_t i = 0; i < *count - 1; i++) // перебор всех стадий
	{
		stage[process] = i + 1;
		turn[i] = process;
		for (size_t k = 0; k < *count; k++)
		{
			if (k == process)
				continue;
			while (stage[k] > i && turn[i] == process) Sleep(S);
		}
	}
}
void critical_section_n_process::leave_region(size_t process)
{
	stage[process] = 0;
}

// barrier_process
barrier_process::barrier_process(const char * name, size_t Count)
{
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 2*sizeof(size_t), name);
	if(hMapping == nullptr)
		Error("Create file mapping failed");
	ibarrier = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(ibarrier == nullptr)
		Error("Create view failed");
	count = ibarrier + 1;
	*ibarrier = 0;
	*count = Count;
}
barrier_process::barrier_process(const char * name)
{
	hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,name);
	if(hMapping == nullptr)
		Error("Open file mapping failed");
	ibarrier = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	if(ibarrier == nullptr)
		Error("Create view failed");
	count = ibarrier + 1;
}
barrier_process::~barrier_process()
{
	if(!UnmapViewOfFile(const_cast<size_t*>(ibarrier)))
		Error("Unmap view failed");
	if(!CloseHandle(hMapping))
		Error("Can't close handle");
}
