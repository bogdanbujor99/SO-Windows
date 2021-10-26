#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>


#define BUF_SIZE 256

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 1)
    {
        cout << "Numar prea mic de argumente";
        return 0;
    }

    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE, TEXT("Global\\mapping"));

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    pBuf = (LPCTSTR)MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,BUF_SIZE);

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        return 0;
    }
    int v[200];
    CopyMemory(v, (PVOID)pBuf ,800);
    int nrVowel = 0;
    int nrConsonant = 0;
    int nrFigure = 0;
    for (int i = 'a'; i <= 'z'; i++)
    {
        if ((i == 'a' || i == 'e' || i == 'i' || i == 'o' || i == 'u') && v[i] != 0)
            nrVowel += v[i];
        else if (v[i] != 0)
            nrConsonant += v[i];
    }
    for (int i = 'A'; i <= 'Z'; i++)
    {
        if ((i == 'A' || i == 'E' || i == 'I' || i == 'O' || i == 'U') && v[i] != 0)
            nrVowel += v[i];
        else if (v[i] != 0)
            nrConsonant += v[i];
    }
    for (int i = '0'; i <= '9'; i++)
    {
        if (v[i] != 0)
            nrFigure += v[i];
    }
    HANDLE hfile;
    if (!(hfile = CreateFile(argv[0], GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    DWORD byte = 0;
    string result = "NR VOWEL: " + to_string(nrVowel) + "\n" + "NR CONSONANT: " + to_string(nrConsonant) + "\n" + "NR FIGURE: " + to_string(nrFigure);
    if (!WriteFile(hfile, result.c_str(), strlen(result.c_str()), &byte, NULL))
    {
        printf("Error: %d", GetLastError());
    }
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
