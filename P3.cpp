#include <iostream>
#include <Windows.h>
#include <accctrl.h>
#include <aclapi.h>
#include <sddl.h>
#pragma warning (disable : 4996)

using namespace std;

int main()
{
	int ok1 = 2, ok2 = 2;
	HANDLE hfile;
	hfile = CreateFile("D:\\RESTRICTED WRITE\\P2.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error to CreateFile: %d", GetLastError());
		exit(0);
	}
	PSID ppsidGroup;
	PSECURITY_DESCRIPTOR pd;
	GetSecurityInfo(hfile, SE_FILE_OBJECT, GROUP_SECURITY_INFORMATION, NULL, &ppsidGroup, NULL, NULL, &pd);
	char* StringSid;
	ConvertSidToStringSidA(ppsidGroup, &StringSid);
	
	CloseHandle(hfile);

	hfile = CreateFile("D:\\RESTRICTED WRITE\\P3.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error to CreateFile: %d", GetLastError());
		ok1 = 0;
	}

	DWORD byte = 0;
	if (!WriteFile(hfile, StringSid, strlen(StringSid), &byte, NULL))
	{
		printf("Error to WriteFile: %d", GetLastError());
		ok1 = 1;
	}
	CloseHandle(hfile);

	HKEY hkey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema2\\P3", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL) != ERROR_SUCCESS)
	{
		printf("ERROR RegCreateKeyEx: %d", GetLastError());
		return 0;
	}

	char copy[256];
	strcpy(copy, StringSid);
	strcpy(copy + (strrchr(copy, '-') - copy), copy + (strrchr(copy, '-') - copy) + 1);
	int val = atoi(copy + strlen(copy) - 6);
	char s[10] = "CSSO";

	if (RegSetValueEx(hkey, s, 0, REG_DWORD, (const BYTE*)&val, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		printf("Error RegSetValueEx: %d", GetLastError());
		ok2 = 0;
	}
	RegCloseKey(hkey);

	hfile = CreateFile("D:\\LOGS\\P3.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error to CreateFile: %d", GetLastError());
		ok2 = 1;
	}

	char str[256];
	if (ok1 == 0)strcpy(str, "Nu a reusit crearea fisierului.\n");
	else if (ok1 == 1) strcpy(str, "Nu a reusit scrierea fisierului.\n");
	else strcpy(str, "S-a reusit scrierea fisierului.\n");

	if (ok2 == 0) strcat(str, "Nu a reusit crearea subkey.\n");
	else if (ok2 == 1) strcat(str, "Nu a reusit setarea valorii.\n");
	else strcat(str, "A reusit setarea valorii.");

	byte = 0;
	if (!WriteFile(hfile, str, strlen(str), &byte, NULL))
	{
		printf("Error to WriteFile1: %d", GetLastError());
	}
	CloseHandle(hfile);
}
