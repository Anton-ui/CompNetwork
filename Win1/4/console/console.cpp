#include <windows.h>
#include <iostream>

using std::cout;
using std::endl;
using std::flush;

const DWORD S = 10;//��

int main(int argc, char** argv)
{
	if(argc != 2)
		exit(1);
	//������� ������
	for(size_t j = atoi(argv[1]) * 10, max = j + 10; j < max; ++j)
	{
		//������� ������ ����� j
        for(size_t i = 0; i < 10; ++i)
		{
			cout << j << ' ' << flush;
			Sleep(S);
		}
		cout << "child" << argv[1] << endl;
        Sleep(S);
  	} 
  return 0;
}