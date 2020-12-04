//c++ 11

#include "..\win11\main.hpp"

int main(int argc, char* argv[])
{
	//Инициализация процесса
	if(argc != 2)
		exit(1);
	size_t num = static_cast<size_t>(strtoull(argv[1],nullptr,10));//Номер процесса потомка
	//Инициализация генератора случайных чисел
	srand(GetTickCount()*(num + 1));
#ifdef SYNC
	critical_section_n_process* csn = new critical_section_n_process("critical_section_n_process"); //Подключение к критической секции
#endif
	//Вывод данные
	for (int j = 0; j < 10; ++j)
	{
		Sleep(rndtm()*10);
#ifdef SYNC
		csn->enter_region(num);//Вход в критическую секцию
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << num << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << " child" << num << endl;
#ifdef SYNC
		//Выход из критической секции
		csn->leave_region(num);
#endif
	}
#ifdef SYNC
	delete csn;//Отключение от критической секции 
#endif
	return 0;
}
