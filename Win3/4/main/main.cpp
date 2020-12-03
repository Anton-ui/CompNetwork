//c++ 11

#include <windows.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::hex;
using std::dec;

void Error(const char*);
void Status(DWORD, const char*);

const size_t N = 10;// ������ �������
const DWORD S = 1000;//��

int main(int argc, char** argv)
{
	DWORD status;
	//��������� ������ ����������� ����� � ������
	HANDLE hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
        PAGE_READWRITE, 0, N * sizeof(size_t), "MappingName");
	if(hMapping == nullptr)
		Error("Create file mapping failed");
	//������� ��� �����
	volatile size_t *ptr = static_cast<volatile size_t*>(MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0));
    if(ptr == nullptr)
		Error("Create view failed");
    //�������������� ������ 
	cout << "Initial state of the array:  ";
    for(size_t i = 0; i < N; ++i)
		cout << (ptr[i] = 0) << ' ' << flush;
	cout << endl;
	//������� �������    
	HANDLE hSemaphore = CreateSemaphore(nullptr, 0, N, "DemoSemaphore");
	if(hSemaphore == nullptr)
		Error("Can't create semaphore");
	//������� �������, ������� ����� ������ ������ �� ������������� � ������ �����
	char  lpszAppName[_MAX_PATH];
	_fullpath( lpszAppName, "..\\console\\console.exe", _MAX_PATH );
    PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if(!CreateProcess(lpszAppName, nullptr, nullptr, nullptr, false,
            0, nullptr, nullptr, &si, &pi))
	    Error("The new process is not created"); 
	//������� ������ �� �������
    Sleep(S);
	cout << "Modified state of the array: ";
    for(size_t i = 0; i < N; i++)
	{
		//�������� ���������� ������
        status = WaitForSingleObject(hSemaphore, INFINITE);
        if(status != WAIT_OBJECT_0)
    		Status(status, "Wait for semaphore failed");
        //
		cout << ptr[i] << ' ' << flush;
        Sleep(S);
	}
	cout << endl;
    //���� ���������� ���������� �������
	status = WaitForSingleObject(pi.hProcess, INFINITE);
	if(status != WAIT_OBJECT_0)
        Status(status, "Wait for process failed");
	//����� �������������� ��������� �������
    cout << "Final state of the array:    ";
    for(size_t i = 0; i < N; i++)
		cout << ptr[i] << ' ' << flush;
	cout << endl;
	//�������� ����������� ����� � ������
	if(!UnmapViewOfFile(const_cast<size_t*>(ptr)))
        Error("Unmap view failed");
	//��������� �����������
	if(!CloseHandle(pi.hThread))
		Error("Can't close handle");
	if(!CloseHandle(pi.hProcess))
		Error("Can't close handle");
	if(!CloseHandle(hMapping))
        Error("Can't close handle");
    if(!CloseHandle(hSemaphore))
        Error("Can't close handle");
    //���� ������� �� ���������� ��������
	cout << "Press enter to exit.";
	cin.get();
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

void Status(DWORD status, const char* str)
{
	cout << str << ", status = 0x" << hex << status
		<< '(' << dec << status << ')'<< endl;
    cout << "Press enter to exit." << endl;
    cin.get();
    exit(status);
}