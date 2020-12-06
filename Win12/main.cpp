//c++ 11

#include "main.hpp"

int main(int argc, char** argv)
{
	//Время запуска процесса
	DWORD tm = GetTickCount();
	//Инициализация генератора случайных чисел
	srand(tm);

#ifdef SYNC
	barrier_process* br = new barrier_process("barrier_process", P);//Создание барьера для P процессов
#endif
	//Создание дочерних процессов
	char lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\Win12_child\\child.exe", _MAX_PATH);
	STARTUPINFO  si[P - 1];
	PROCESS_INFORMATION  pi[P - 1];
	HANDLE hProcess[P - 1];
	char (*args)[21] = new char[P-1][21];
	for(size_t i = 0; i < P - 1; i++)
	{
		sprintf(args[i], "%s %zu %lu", "child", i + 1, tm);
		ZeroMemory(si + i, sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		if (!CreateProcess(lpszAppName, args[i], nullptr, nullptr, false,
                0, nullptr, nullptr, si + i, pi + i))
            Error("The new process is not created");
		hProcess[i] = pi[i].hProcess;
	}
	//Вывод данных
	Sleep(rndtm());
	cout << "main     enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(0);//Синхронизация
#endif
	cout << "main     leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	Sleep(1000);
	cout << endl;
#ifdef SYNC
	br->reset();//Сброс барьера
#endif	
	Sleep(rndtm());	
	cout << "main     enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
		br->wait(0);//Синхронизация
#endif
	cout << "main     leave barrier:    " << GetTickCount() - tm << " ms"  << endl;
	Sleep(1000);
	cout << endl;
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
	delete br;//Уничтожение барьера
#endif
	//Ожидание ввода команды на завершение процесса
	cout << "Press enter to exit." << endl;
	cin.get();
	return 0;
}
