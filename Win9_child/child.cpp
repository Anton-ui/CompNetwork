//c++ 11

#include "..\main\main.hpp"

int main(int argc, char* argv[])
{
	//������������� ��������
	if(argc != 2)
		exit(1);
	size_t num = static_cast<size_t>(strtoull(argv[1],nullptr,10));//����� �������� �������
	//������������� ���������� ��������� ����� 
	srand(GetTickCount()*(num + 1));
#ifdef SYNC
	rotation_process* rt = new rotation_process("rotation_process");//����������� � ������� "�����������"
#endif
	//����� ������
	for (size_t i = 1 + num; i <= N; i+=P)
	{
		Sleep(rndtm());
#ifdef SYNC
		rt -> enter_region(num);//�������������
#endif
		cout << i << endl;
#ifdef SYNC
		rt -> leave_region();//�������������
#endif
	}
#ifdef SYNC
	delete rt;//���������� �� ������� "�����������"
#endif
	return 0;
}

