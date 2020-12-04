//c++ 11

#include "..\win11\main.hpp"

int main(int argc, char* argv[])
{
	//������������� ��������
	if(argc != 2)
		exit(1);
	size_t num = static_cast<size_t>(strtoull(argv[1],nullptr,10));//����� �������� �������
	//������������� ���������� ��������� �����
	srand(GetTickCount()*(num + 1));
#ifdef SYNC
	critical_section_n_process* csn = new critical_section_n_process("critical_section_n_process"); //����������� � ����������� ������
#endif
	//����� ������
	for (int j = 0; j < 10; ++j)
	{
		Sleep(rndtm()*10);
#ifdef SYNC
		csn->enter_region(num);//���� � ����������� ������
#endif
		for (size_t i = 0; i < 10; ++i)
		{
			cout << num << j << ' ' << flush;
			Sleep(rndtm());
		}
		cout << " child" << num << endl;
#ifdef SYNC
		//����� �� ����������� ������
		csn->leave_region(num);
#endif
	}
#ifdef SYNC
	delete csn;//���������� �� ����������� ������ 
#endif
	return 0;
}
