#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUF_SIZE 256

using namespace std;
int v[200];

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        cout << "Numar prea mic de argumente";
        return 0;
    }
    HANDLE hfile;
    char s[256];
    if (!(hfile = CreateFile("C:\\Users\\bujor\\source\\repos\\input.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    DWORD byte = 0;
    LARGE_INTEGER fileSize;
    if (GetFileSizeEx(hfile, &fileSize) == INVALID_FILE_SIZE)
    {
        printf("Error: %d", GetLastError());
    }
    if (!ReadFile(hfile, s, 255, &byte, NULL))
    {
        printf("Error: %d", GetLastError());
    };
    s[fileSize.QuadPart] = '\0';
    for (int i = 0; i <= fileSize.QuadPart - 1; i++)
        v[s[i]]++;
    HANDLE hMapFile;
    LPCTSTR pBuf;
    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, TEXT("Global\\mapping"));
    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }
    pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }
    CopyMemory((PVOID)pBuf, v, sizeof(v));
    _getch();
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return 1;
}
