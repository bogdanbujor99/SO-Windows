#include <iostream>
#include <Windows.h>
#include <accctrl.h>
#include <aclapi.h>
#include <sddl.h>
#pragma warning (disable : 4996)

using namespace std;

int main()
{
	int ok1=2, ok2=2, ok3=1;
	DWORD computerSize = 256;
	LPSTR computerName = new char[256];
	GetComputerName(computerName, &computerSize);
	DWORD userSize = 256;
	LPSTR userName = new char[256];
	GetUserName(userName, &userSize);
	char* computerAndUser = new char[computerSize + userSize + 5];
	strcpy(computerAndUser, computerName);
	strcat(computerAndUser, "\\");
	strcat(computerAndUser, userName);
	char domain[256];
	DWORD  domainSize = strlen(domain);
	BYTE userSid[SECURITY_MAX_SID_SIZE];
	DWORD sidSize = SECURITY_MAX_SID_SIZE;
	SID_NAME_USE nameUser;
	LookupAccountNameA(NULL, computerAndUser, userSid, &sidSize, domain, &domainSize, &nameUser);
	LPSTR userSidStr = new char[300];
	ConvertSidToStringSid(&userSid, &userSidStr);

	HANDLE hfile;
	hfile = CreateFile("D:\\RESTRICTED WRITE\\P2.txt", FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE )
	{
		printf("Error to CreateFile: %d", GetLastError());
		ok1 = 0;
	}
	DWORD byte = 0;
	if (!WriteFile(hfile, userSidStr, strlen(userSidStr), &byte, NULL))
	{
		printf("Error to WriteFile: %d", GetLastError());
		ok1 = 1;
	}
	CloseHandle(hfile);

	HKEY hkey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema2\\P2", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL) != ERROR_SUCCESS)
	{
		printf("ERROR RegCreateKeyEx: %d", GetLastError());
		ok2 = 0;
	}

	char copy[256];
	strcpy(copy, userSidStr);
	strcpy(copy + (strrchr(copy, '-') - copy), copy + (strrchr(copy, '-') - copy) + 1);
	int val = atoi(copy + strlen(copy) - 6);
	char s[10] = "CSSO";

	if (RegSetValueEx(hkey, s, 0, REG_DWORD, (const BYTE*)&val , sizeof(DWORD)) != ERROR_SUCCESS)
	{
		printf("Error RegSetValueEx: %d", GetLastError());
		ok2 = 1;
	}
	RegCloseKey(hkey);

	hfile = CreateFile("D:\\LOGS\\P2.txt", FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error: %d", GetLastError());
		exit(0);
	}

	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(STARTUPINFO);
	if (CreateProcess("C:\\Users\\bujor\\source\\repos\\P3\\x64\\Debug\\P3.exe", NULL, NULL, NULL, FALSE, NULL, 0, 0, &startInfo, &processInfo) == 0)
	{
		printf("Error to CreateProcess: %d", GetLastError());
		ok3 = 0;
	}

	char str[256];
	if (ok1 == 0)strcpy(str, "Nu a reusit crearea fisierului.\n");
	else if (ok1 == 1) strcpy(str, "Nu a reusit scrierea fisierului.\n");
	else strcpy(str, "S-a reusit scrierea fisierului.\n");

	if (ok2 == 0) strcat(str, "Nu a reusit crearea subkey.\n");
	else if (ok2 == 1) strcat(str, "Nu a reusit setarea valorii.\n");
	else strcat(str, "A reusit setarea valorii.\n");

	if(ok3==0) strcat(str, "Nu a reusit pornirea procesului.\n");
	else strcat(str, "A reusit pornirea procesului.\n");

	byte = 0;
	if (!WriteFile(hfile, str, strlen(str), &byte, NULL))
	{
		printf("Error to WriteFile: %d", GetLastError());
		ok1 = 1;
	}
	CloseHandle(hfile);
}
