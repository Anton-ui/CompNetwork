#include <windows.h>
#include <iostream>

using std::cout;
using std::endl;
using std::flush;

const DWORD S = 10;//��

int main(int argc, char** argv)
{
	//������� ������
	for(size_t j = 10; j < 20; ++j)
	{
		//������� ������ ����� j
        for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(S);
		}
		cout << "child" << endl;
		Sleep(S*10);
  	} 
  return 0;
}