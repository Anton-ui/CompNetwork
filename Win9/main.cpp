//c++ 11

#include "main.hpp"

int main(int argc, char** argv)
{
	//Инициализация генератора случайных чисел 
	srand(GetTickCount());
#ifdef SYNC
	rotation_process* rt = new rotation_process("rotation_process", P);//Создание объекта "чередование для Р процессов"
#endif
	//Создание дочерних процессов
	char lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\Win9_child\\child.exe", _MAX_PATH);
	STARTUPINFO  si[P - 1];
	PROCESS_INFORMATION  pi[P - 1];
	HANDLE hProcess[P - 1];
	char (*args)[11] = new char[P - 1][11];
	for(size_t i = 0; i < P - 1; i++)
	{
		sprintf(args[i], "%s %zu", "child", i + 1);
		ZeroMemory(si + i, sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		if (!CreateProcessA(lpszAppName, args[i], nullptr, nullptr, false,
                0, nullptr, nullptr, si + i, pi + i))
            Error("The new process is not created");
		hProcess[i] = pi[i].hProcess;
	}
	//Вывод данных
	for (size_t i = 1; i <= N; i+=P)
	{
		Sleep(rndtm());
#ifdef SYNC
		rt->enter_region(0);//Синхронизация
#endif
		cout << i << " " << "main" << endl;
#ifdef SYNC
		rt->leave_region();//Синхронизация
#endif
	}
	//Ожидание завершения дочерних процессов
	DWORD status = WaitForMultipleObjects(P - 1, hProcess, true, INFINITE);
    if(status == WAIT_FAILED)
        Status(status, "Wait for proceses failed");
	//Закрытие дескрипторов
	for(size_t i = 0; i < P - 1; i++)
	{
		if(!CloseHandle(pi[i].hThread))
            Error("Can't close handle");
        if(!CloseHandle(pi[i].hProcess))
            Error("Can't close handle");
	}
	delete[] args;
#ifdef SYNC
	delete rt;//Уничтожение объекта "чередование"
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit.";
	cin.get();
	return 0;
}
