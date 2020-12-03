//c++ 11

#include <windows.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;

void Error(const char*);

const size_t N = 10;// ������ �������
const DWORD S = 1500;//��

int main(int argc, char** argv)
{
	//��������� ������ ����������� ����� � ������
	HANDLE hMapping = OpenFileMapping(FILE_MAP_WRITE, false, "MappingName"); 
	if(!hMapping)
		Error("Open file mapping failed");
	//������� ��� �����
	volatile size_t *ptr = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0));
    if(ptr == nullptr)
		Error("Create view failed");
	//��������� ������
	for(size_t i = 0; i < N; i++)
	{
		ptr[i] = i + 1;
		Sleep(S);
	}
	//�������� ����������� ����� � ������
	if(!UnmapViewOfFile(const_cast<size_t*>(ptr)))
        Error("Unmap view failed");
	//��������� ����������
	if(!CloseHandle(hMapping))
        Error("Can't close handle");
	return 0;
}

void Error(const char* str)
{
	DWORD error = GetLastError();
	cout << str << ", error = 0x" << hex << error 
		<< '(' << dec << error << ')' << endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(error);
}