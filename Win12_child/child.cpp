//c++ 11

#include "..\main\main.hpp"

int main(int argc, char* argv[])
{
	//������������� ��������
	if(argc != 3)
		exit(1);
	size_t num = static_cast<size_t>(strtoull(argv[1],nullptr,10));//����� �������� �������
	DWORD tm = strtoul(argv[2],nullptr,10);// ����� ������� ������������� ��������
	const char *vr;
	if(num < 10)
		vr = "  ";
	else
		vr = " ";
	//������������� ���������� ��������� �����
	srand(tm * (num + 1));
#ifdef SYNC
	barrier_process* br = new barrier_process("barrier_process");//����������� � �������
#endif
	//����� ������
	Sleep(rndtm());
	cout << "child" << vr << num << " enter in barrier: " <<  GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(num);//�������������
#endif
	DWORD tmt = GetTickCount() - tm;
	Sleep(30 * num);
	cout << "child" << vr << num << " leave barrier:    " << tmt << " ms"  << endl;
	Sleep(rndtm());
	cout << "child" << vr << num << " enter in barrier: " << GetTickCount() - tm << " ms"  << endl;
#ifdef SYNC
	br->wait(num);//�������������
#endif
	tmt = GetTickCount() - tm;
	Sleep(30*num);
	cout << "child" << vr << num << " leave barrier:    " << tmt << " ms"  << endl;
#ifdef SYNC
	delete br;//���������� �� �������
#endif
	return 0;
}

