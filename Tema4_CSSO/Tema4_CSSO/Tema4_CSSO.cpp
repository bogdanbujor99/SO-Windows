#include <iostream>
#include <string>
#include <Windows.h>
#include <WinInet.h>
#include <strsafe.h>
#pragma comment(lib, "Wininet")
#pragma warning (disable : 4996)

using namespace std;

HINTERNET hInternetOpen;
HINTERNET hInternetConnect;
HINTERNET hHttpOpenRequest;
HANDLE hfileConfig;
HANDLE hfileExecutabile;
DWORD bytesContentConfig = 0;
DWORD bytesContentExecutabile = 0;


void request(char* path)
{
    hInternetOpen = InternetOpen("310910401RSL191039", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternetOpen == NULL)
    {
        printf("Error to InternetOpen %d", GetLastError());
        exit(0);
    }
    hInternetConnect = InternetConnect(hInternetOpen, "cssohw.herokuapp.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
    if (hInternetConnect == NULL)
    {
        printf("Error to InternetConnect %d", GetLastError());
        exit(0);
    }
    PCTSTR rgpszAcceptTypes[] = { (LPCSTR)("text/*"), NULL };
    if(strcmp(path,"config")==0)
        hHttpOpenRequest = HttpOpenRequest(hInternetConnect, "GET", "/config/310910401RSL191039", "HTTP/1.1", NULL, rgpszAcceptTypes, NULL, NULL);
    else 
        hHttpOpenRequest = HttpOpenRequest(hInternetConnect, "GET", "/executabile/310910401RSL191039", "HTTP/1.1", NULL, rgpszAcceptTypes, NULL, NULL);
    if (hHttpOpenRequest == NULL)
    {
        printf("Error to HttpOpenRequest %d", GetLastError());
        exit(0);
    }
    if (!HttpSendRequest(hHttpOpenRequest, NULL, NULL, NULL, 0))
    {
        printf("Error to HttpSendRequest %d", GetLastError());
        exit(0);
    }
    char* buf = new char[4096], * content = new char[50000];
    DWORD byte = 0, bytesContent = 0;
    if (!InternetReadFile(hHttpOpenRequest, buf, 4096, &byte))
    {
        printf("Error to InternetReadFile %d", GetLastError());
        exit(0);
    }
    while (byte != 0)
    {
        memcpy(content + bytesContent, buf, byte);
        memset(buf, NULL, 4096);
        bytesContent += byte;
        byte = 0;
        if (!InternetReadFile(hHttpOpenRequest, buf, 4096, &byte))
        {
            printf("Error to InternetReadFile %d", GetLastError());
            exit(0);
        }
    }
    content[bytesContent] = NULL;
    if (!CreateDirectory("C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4", NULL))
    {
        printf("Error to CreateDirectory 1: %d", GetLastError());
    }
    if (strcmp(path, "config") == 0)
    {
        if (!(hfileConfig = CreateFile("C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\config.txt", FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
        {
            printf("Error: %d", GetLastError());
            exit(0);
        }
        byte = 0;
        if (!WriteFile(hfileConfig, content, strlen(content), &byte, NULL))
        {
            printf("Error: %d", GetLastError());
            exit(0);
        }
    }
    else
    {
        if (!(hfileExecutabile = CreateFile("C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\executabile.txt", FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
        {
            printf("Error : %d", GetLastError());
            exit(0);
        }
        byte = 0;
        if (!WriteFile(hfileExecutabile, content, strlen(content), &byte, NULL))
        {
            printf("Error: %d", GetLastError());
            exit(0);
        }
    }
    if (strcmp(path, "config") == 0)
        bytesContentConfig = bytesContent;
    else
        bytesContentExecutabile = bytesContent;
    InternetCloseHandle(hHttpOpenRequest);
    InternetCloseHandle(hInternetConnect);
    InternetCloseHandle(hInternetOpen);
    if (strcmp(path, "config") == 0)
        CloseHandle(hfileConfig);
    else 
        CloseHandle(hfileExecutabile);
}

void createDirectorFromConfig()
{
    char *s = new char[bytesContentConfig];
    DWORD byte = 0;
    if (!(hfileConfig = CreateFile("C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\config.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
    }
    if (!ReadFile(hfileConfig, s, bytesContentConfig, &byte, NULL))
    {
        printf("Error to ReadFile: %d", GetLastError());
        exit(0);
    }
    s[bytesContentConfig] = NULL;
    char* pch;
    pch = strtok(s,"\n");
    while (pch != NULL)
    {
        string path = "C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\" + string(pch);
        if (!CreateDirectory(path.c_str(), NULL))
        {
            printf("Error to CreateDirectory2: %d", GetLastError());
        }
        pch = strtok(NULL,"\n");
    }
    CloseHandle(hfileConfig);
}

void createFileFromExecutabile()
{
    char* s = new char[bytesContentExecutabile];
    DWORD byte = 0;
    if (!(hfileExecutabile = CreateFile("C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\executabile.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
    }
    if (!ReadFile(hfileExecutabile, s, bytesContentExecutabile, &byte, NULL))
    {
        printf("Error to ReadFile: %d", GetLastError());
        exit(0);
    }
    s[bytesContentExecutabile] = NULL;
    char* pch;
    pch = strtok(s, "\n");
    while (pch != NULL)
    {
        char* pozFile;
        pozFile = strstr(pch, "||");
        char directory[33];
        strncpy(directory, pch, pozFile - pch);
        directory[pozFile - pch] = NULL;
        char* work = strstr(pozFile, "//");
        char* lpszObjectName = strstr(work+2, "/");
        char lpszServerName[100];
        strncpy(lpszServerName, work + 2, lpszObjectName - work-2);
        lpszServerName[lpszObjectName - work - 2] = NULL;
        char *path = strstr(work + 2, "/");
        path = strrchr(path, '/');

        HANDLE hfile;
        string pathFile = "C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\" + string(directory) + "\\" + string(path+1);
        if (!(hfile = CreateFile(pathFile.c_str(), FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
        {
            printf("Error : %d", GetLastError());
            exit(0);
        }

        hInternetOpen = InternetOpen("310910401RSL191039", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (hInternetOpen == NULL)
        {
            printf("Error to InternetOpen %d", GetLastError());
            exit(0);
        }
        hInternetConnect = InternetConnect(hInternetOpen, lpszServerName, 80, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
        if (hInternetConnect == NULL)
        {
            printf("Error to InternetConnect %d", GetLastError());
            exit(0);
        }
        PCTSTR rgpszAcceptTypes[] = { (LPCSTR)("text/*"), NULL };
        hHttpOpenRequest = HttpOpenRequest(hInternetConnect, "GET", lpszObjectName, "HTTP/1.1", NULL, rgpszAcceptTypes, NULL, NULL);
        if (hHttpOpenRequest == NULL)
        {
            printf("Error to HttpOpenRequest %d", GetLastError());
            exit(0);
        }
        if (!HttpSendRequest(hHttpOpenRequest, NULL, NULL, NULL, 0))
        {
            printf("Error to HttpSendRequest %d", GetLastError());
            exit(0);
        }
        char* buf = new char[4096];
        DWORD byte = 0;
        if (!InternetReadFile(hHttpOpenRequest, buf, 4096, &byte))
        {
            printf("Error to InternetReadFile %d", GetLastError());
            exit(0);
        }
        while (byte != 0)
        {
            buf[byte] = NULL;
            if (!WriteFile(hfile, buf, strlen(buf), &byte, NULL))
            {
                printf("Error: %d", GetLastError());
                exit(0);
            }
            memset(buf, NULL, 4096);
            byte = 0;
            if (!InternetReadFile(hHttpOpenRequest, buf, 4096, &byte))
            {
                printf("Error to InternetReadFile %d", GetLastError());
                exit(0);
            }
        }
        pch = strtok(NULL, "\n");
        CloseHandle(hfile);
    }
    InternetCloseHandle(hHttpOpenRequest);
    InternetCloseHandle(hInternetConnect);
    InternetCloseHandle(hInternetOpen);
    CloseHandle(hfileExecutabile);
}

void calculateSizeAndNrFiles()
{
    int size=0;
    int nr_files = 0;
    WIN32_FIND_DATA ffd;
    TCHAR szDir[MAX_PATH];
    StringCchCopy(szDir, MAX_PATH, "C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4");
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    HANDLE hFind = FindFirstFile(szDir, &ffd);
    FindNextFile(hFind, &ffd);
    FindNextFile(hFind, &ffd);
    do
    {
            TCHAR szDir1[MAX_PATH];
            string path = "C:\\Users\\bujor\\source\\repos\\CSSO_Tema_4\\" + string(ffd.cFileName);
            StringCchCopy(szDir1, MAX_PATH, path.c_str());
            StringCchCat(szDir1, MAX_PATH, TEXT("\\*"));
            HANDLE hFind1 = FindFirstFile(szDir1, &ffd);
            do
            {
                if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    cout << path << " " << ffd.cFileName << " " << ffd.nFileSizeLow << endl;
                    size += ffd.nFileSizeLow;
                    nr_files++;
                }
            } while (FindNextFile(hFind1, &ffd) != 0);
    } while (FindNextFile(hFind, &ffd) != 0);

    string format = "id=310910401RSL191039&size=" + to_string(size) + "&s=" + to_string(nr_files) + "&e=0";
    cout << format;
    hInternetOpen = InternetOpen("310910401RSL191039", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternetOpen == NULL)
    {
        printf("Error to InternetOpen %d", GetLastError());
        exit(0);
    }
    hInternetConnect = InternetConnect(hInternetOpen, "cssohw.herokuapp.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
    if (hInternetConnect == NULL)
    {
        printf("Error to InternetConnect %d", GetLastError());
        exit(0);
    }
    PCTSTR rgpszAcceptTypes[] = { (LPCSTR)("text/*"), NULL };
    hHttpOpenRequest = HttpOpenRequest(hInternetConnect, "POST", "/submit", "HTTP/1.1", NULL, rgpszAcceptTypes, NULL, NULL);
    if (hHttpOpenRequest == NULL)
    {
        printf("Error to HttpOpenRequest %d", GetLastError());
        exit(0);
    }
    if (!HttpSendRequest(hHttpOpenRequest, "Content-Type: application/x-www-form-urlencoded", strlen("Content-Type: application/x-www-form-urlencoded"), (LPVOID)format.c_str(), format.length()))
    {
        printf("Error to HttpSendRequest %d", GetLastError());
        exit(0);
    }
    DWORD byte;
    char *s = new char[4096];
    if (!InternetReadFile(hHttpOpenRequest, s, 4096, &byte))
    {
        printf("Error to InternetReadFile %d", GetLastError());
        exit(0);
    }
    s[byte] = NULL;
    cout << s;
    InternetCloseHandle(hHttpOpenRequest);
    InternetCloseHandle(hInternetConnect);
    InternetCloseHandle(hInternetOpen);
    CloseHandle(hFind);
}

int main()
{
    request((char*)"config");
    request((char*)"executabile");
    createDirectorFromConfig();
    createFileFromExecutabile();
    calculateSizeAndNrFiles();
}
