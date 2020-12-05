//c++ 11

#include "..\main\main.hpp"

int main(int argc, char* argv[])
{
	//Инициализация процесса
	if(argc != 3)
		exit(1);
	size_t num = static_cast<size_t>(strtoull(argv[1],nullptr,10));//Номер процесса потомка
	DWORD tm = strtoul(argv[2],nullptr,10);// Время запуска родительского процесса
	const char *vr;
	if(num < 10)
		vr = "  ";
	else
		vr = " ";
	//Инициализация генератора случайных чисел
	srand(tm * (num + 1));
#ifdef SYNC
	barrier_process* br = new barrier_process("barrier_process");//Подключение к барьеру
#endif
	//Вывод данных
	Sleep(rndtm());
	cout << "child" << vr << num << " enter in barrier: " <<  GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(num);//Синхронизация
#endif
	DWORD tmt = GetTickCount() - tm;
	Sleep(30 * num);
	cout << "child" << vr << num << " leave barrier:    " << tmt << " ms"  << endl;
	Sleep(rndtm());
	cout << "child" << vr << num << " enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(num);//Синхронизация
#endif
	tmt = GetTickCount() - tm;
	Sleep(30*num);
	cout << "child" << vr << num << " leave barrier:    " << tmt << " ms"  << endl;
#ifdef SYNC
	delete br;//Отключение от барьера
#endif
	return 0;
}

