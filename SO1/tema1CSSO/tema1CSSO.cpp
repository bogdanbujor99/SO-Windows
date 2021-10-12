#include <windows.h>
#include <stdio.h>
#include <string>
#include<iostream>
#include<cstring>
#pragma warning (disable : 4996)
using namespace std;

struct regInfo {
    DWORD lpcSubKeys;
    DWORD lpcMaxSubKeyLen;
    DWORD lpcMaxClassLen;
    DWORD lpcValues;
    DWORD lpcMaxValueNameLen;
    DWORD lpcMaxValueLen;
    DWORD lpcbSecurityDescriptor;
    FILETIME lpftLastWriteTime;
}reg;

string info(regInfo reg){
    string str;
    str += "lpcSubKeys " + to_string(reg.lpcSubKeys) + "\n";
    str += "lpcMaxSubKeyLen " + to_string(reg.lpcMaxSubKeyLen) + "\n";
    str += "lpcMaxClassLen " + to_string(reg.lpcMaxClassLen) + "\n";
    str += "lpcValues " + to_string(reg.lpcValues) + "\n";
    str += "lpcMaxValueNameLen " + to_string(reg.lpcMaxValueNameLen) + "\n";
    str += "lpcMaxValueLen " + to_string(reg.lpcMaxValueLen) + "\n";
    str += "lpcbSecurityDescriptor " + to_string(reg.lpcbSecurityDescriptor) + "\n";
    str += "lpftLastWriteTimeLow " + to_string(reg.lpftLastWriteTime.dwLowDateTime) + "\n";
    str += "lpftLastWriteTimeHigh " + to_string(reg.lpftLastWriteTime.dwHighDateTime);
    return str;
}

int main()
{
    if (!CreateDirectory("D:\\CSSO", NULL) || !CreateDirectory("D:\\CSSO\\Laboratoare", NULL) || !CreateDirectory("D:\\CSSO\\Laboratoare\\Tema1", NULL))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    HANDLE sumar;
    if (!(sumar = CreateFile("D:\\CSSO\\Laboratoare\\Tema1\\sumar.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    HKEY key = NULL;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, NULL, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        printf("Error4: %d", GetLastError());
        return 0;
    }
    else {
        if (RegQueryInfoKey(key, NULL, NULL, NULL,
            &reg.lpcSubKeys,
            &reg.lpcMaxSubKeyLen,
            &reg.lpcMaxClassLen,
            &reg.lpcValues,
            &reg.lpcMaxValueNameLen,
            &reg.lpcMaxValueLen,
            &reg.lpcbSecurityDescriptor,
            &reg.lpftLastWriteTime)) 
        {
            printf("Error3: %d", GetLastError());
        }
        else {
            HANDLE file;
            if (!(file=CreateFile("D:\\CSSO\\Laboratoare\\Tema1\\HKLM.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
                printf("Error: %d", GetLastError());
            }
            else 
            {
                DWORD byte = 0 ;
                string str = info(reg);
                if (!WriteFile(file, (char*)str.c_str(), strlen((char*)str.c_str()), &byte, NULL))
                {
                   printf("Error: %d", GetLastError());
                };
                LARGE_INTEGER fileSize;
                if (GetFileSizeEx(file, &fileSize)== INVALID_FILE_SIZE)
                {
                    printf("Error: %d", GetLastError());
                }
                char filePath[100];
                GetFinalPathNameByHandleA(file, filePath, 100, FILE_NAME_NORMALIZED);
                string strSumar = string(&filePath[4]) + " : " + to_string(fileSize.QuadPart) + "\n";
                if (!WriteFile(sumar, (char*)strSumar.c_str(), strlen((char*)strSumar.c_str()),&byte,NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                RegCloseKey(key);
            }
        }
    }

    if (RegOpenKeyEx(HKEY_CURRENT_CONFIG, NULL, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        printf("Error: %d", GetLastError());
        return 0;
    }
    else {
        if (RegQueryInfoKey(key, NULL, NULL, NULL,
            &reg.lpcSubKeys,
            &reg.lpcMaxSubKeyLen,
            &reg.lpcMaxClassLen,
            &reg.lpcValues,
            &reg.lpcMaxValueNameLen,
            &reg.lpcMaxValueLen,
            &reg.lpcbSecurityDescriptor,
            &reg.lpftLastWriteTime))
        {
            printf("Error: %d", GetLastError());
        }
        else {
            HANDLE file;
            if (!(file = CreateFile("D:\\CSSO\\Laboratoare\\Tema1\\HKCC.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
                printf("Error: %d", GetLastError());
            }
            else
            {
                DWORD byte = 0;
                string str = info(reg);
                if (!WriteFile(file, (char*)str.c_str(), strlen((char*)str.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                LARGE_INTEGER fileSize;
                if (GetFileSizeEx(file, &fileSize) == INVALID_FILE_SIZE)
                {
                    printf("Error: %d", GetLastError());
                }
                char filePath[100];
                GetFinalPathNameByHandleA(file, filePath, 100, FILE_NAME_NORMALIZED);
                string strSumar = string(&filePath[4]) + " : " + to_string(fileSize.QuadPart) + "\n";
                if (!WriteFile(sumar, (char*)strSumar.c_str(), strlen((char*)strSumar.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                RegCloseKey(key);
            }
        }
    }

    if (RegOpenKeyEx(HKEY_CLASSES_ROOT, NULL, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        printf("Error: %d", GetLastError());
        return 0;
    }
    else {
        if (RegQueryInfoKey(key, NULL, NULL, NULL,
            &reg.lpcSubKeys,
            &reg.lpcMaxSubKeyLen,
            &reg.lpcMaxClassLen,
            &reg.lpcValues,
            &reg.lpcMaxValueNameLen,
            &reg.lpcMaxValueLen,
            &reg.lpcbSecurityDescriptor,
            &reg.lpftLastWriteTime))
        {
            printf("Error: %d", GetLastError());
        }
        else {
            HANDLE file;
            if (!(file = CreateFile("D:\\CSSO\\Laboratoare\\Tema1\\HKCR.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
                printf("Error: %d", GetLastError());
            }
            else
            {
                DWORD byte = 0;
                string str = info(reg);
                if (!WriteFile(file, (char*)str.c_str(), strlen((char*)str.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                LARGE_INTEGER fileSize;
                if (GetFileSizeEx(file, &fileSize) == INVALID_FILE_SIZE)
                {
                    printf("Error: %d", GetLastError());
                }
                char filePath[100];
                GetFinalPathNameByHandleA(file, filePath, 100, FILE_NAME_NORMALIZED);
                string strSumar = string(&filePath[4]) + " : " + to_string(fileSize.QuadPart) + "\n";
                if (!WriteFile(sumar, (char*)strSumar.c_str(), strlen((char*)strSumar.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                RegCloseKey(key);
            }
        }
    }

    if (RegOpenKeyEx(HKEY_CURRENT_USER, NULL, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        printf("Error: %d", GetLastError());
        return 0;
    }
    else {
        if (RegQueryInfoKey(key, NULL, NULL, NULL,
            &reg.lpcSubKeys,
            &reg.lpcMaxSubKeyLen,
            &reg.lpcMaxClassLen,
            &reg.lpcValues,
            &reg.lpcMaxValueNameLen,
            &reg.lpcMaxValueLen,
            &reg.lpcbSecurityDescriptor,
            &reg.lpftLastWriteTime))
        {
            printf("Error: %d", GetLastError());
        }
        else {
            HANDLE file;
            if (!(file = CreateFile("D:\\CSSO\\Laboratoare\\Tema1\\HKCU.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
                printf("Error: %d", GetLastError());
            }
            else
            {
                DWORD byte = 0;
                string str = info(reg);
                if (!WriteFile(file, (char*)str.c_str(), strlen((char*)str.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                LARGE_INTEGER fileSize;
                if (GetFileSizeEx(file, &fileSize) == INVALID_FILE_SIZE)
                {
                    printf("Error: %d", GetLastError());
                }
                char filePath[100];
                GetFinalPathNameByHandleA(file, filePath, 100, FILE_NAME_NORMALIZED);
                string strSumar = string(&filePath[4]) + " : " + to_string(fileSize.QuadPart) + "\n";
                if (!WriteFile(sumar, (char*)strSumar.c_str(), strlen((char*)strSumar.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                RegCloseKey(key);
            }
        }
    }

    if (RegOpenKeyEx(HKEY_USERS, NULL, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        printf("Error: %d", GetLastError());
        return 0;
    }
    else {
        if (RegQueryInfoKey(key, NULL, NULL, NULL,
            &reg.lpcSubKeys,
            &reg.lpcMaxSubKeyLen,
            &reg.lpcMaxClassLen,
            &reg.lpcValues,
            &reg.lpcMaxValueNameLen,
            &reg.lpcMaxValueLen,
            &reg.lpcbSecurityDescriptor,
            &reg.lpftLastWriteTime))
        {
            printf("Error: %d", GetLastError());
        }
        else {
            HANDLE file;
            if (!(file = CreateFile("D:\\CSSO\\Laboratoare\\Tema1\\HKU.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
                printf("Error: %d", GetLastError());
            }
            else
            {
                DWORD byte = 0;
                string str = info(reg);
                if (!WriteFile(file, (char*)str.c_str(), strlen((char*)str.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                LARGE_INTEGER fileSize;
                if (GetFileSizeEx(file, &fileSize) == INVALID_FILE_SIZE)
                {
                    printf("Error: %d", GetLastError());
                }
                char filePath[100];
                GetFinalPathNameByHandleA(file, filePath, 100, FILE_NAME_NORMALIZED);
                string strSumar = string(&filePath[4]) + " : " + to_string(fileSize.QuadPart) + "\n";
                if (!WriteFile(sumar, (char*)strSumar.c_str(), strlen((char*)strSumar.c_str()), &byte, NULL))
                {
                    printf("Error: %d", GetLastError());
                };
                RegCloseKey(key);
            }
        }
    }

    DWORD d;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema1", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &d) != ERROR_SUCCESS)
    {
        printf("Error: %d", GetLastError());
        return 0;
    }

    char filePath[100];
    if (!GetFinalPathNameByHandle(sumar, filePath, 100, FILE_NAME_NORMALIZED))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }

    DWORD size;
    if (RegSetValueEx(key, &filePath[4], 0, REG_SZ, (const BYTE*)filePath, strlen(filePath)) != ERROR_SUCCESS)
    {
        printf("Error: %d", GetLastError());
        return 0;
    }

    LARGE_INTEGER sumarSize;
    if (GetFileSizeEx(sumar, &sumarSize) == INVALID_FILE_SIZE)
    {
        printf("Error: %d", GetLastError());
        return 0;
    }

    if (RegSetValueEx(key, (char*)(to_string(sumarSize.QuadPart)).c_str(), 0, REG_DWORD, (const BYTE*)&sumarSize.QuadPart, sizeof(DWORD)) != ERROR_SUCCESS)
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    RegCloseKey(key);
    return 0;
}