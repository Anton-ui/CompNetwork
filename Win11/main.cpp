//c++ 11

#include "main.hpp"

int main(int argc, char** argv)
{
	//Инициализация генератора случайных чисел
	srand(GetTickCount());
#ifdef SYNC
	critical_section_n_process* csn = new critical_section_n_process("critical_section_n_process",P); //Создание критической секции для P процессов
#endif
	//Создание дочерних процессов
	char lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\Win11_child\\child.exe", _MAX_PATH);
	STARTUPINFO  si[P-1];
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
	for (size_t j = 0; j < 10; ++j)
	{
		Sleep(rndtm()*10);
#ifdef SYNC
		csn->enter_region(0);//Вход в критическую секцию
#endif
		for (int i = 0; i < 10; ++i)
		{
			cout << ' ' << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << " parent"  << endl;
#ifdef SYNC
		csn->leave_region(0);//Выход из критической секции
#endif
	}
    //Ожидание завершения дочерних прцессов
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
	delete csn;//Уничтожение критической секции
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit.";
	cin.get();
	return 0;
}


