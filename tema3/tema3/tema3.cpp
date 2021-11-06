#include <windows.h>
#include <string>
#include <tchar.h>
#include <strsafe.h>
#include <fstream>
#include <iostream>
#include <algorithm>

#define WORKER_THREADS_COUNT 5
#define ITERATIONS 60
#define BUF_SIZE 256
#define MAX_SIZE 256

using namespace std;

DWORD WINAPI RCM(LPVOID nrThread);

void MyErrorHandler(LPTSTR lpszFunction);

int time_out = 0;

DWORD   dwThreadIdArray[WORKER_THREADS_COUNT];
HANDLE  hThreadArray[WORKER_THREADS_COUNT];
HANDLE anonMutex[WORKER_THREADS_COUNT];
HANDLE rowToMutexFile; 
HANDLE hEvent;
HANDLE hMapFileOut;
LPCTSTR pBufOut;
HANDLE hMapFileIn;
LPCTSTR pBufIn;
LPCTSTR auxpBufIn;
HANDLE hfile;
HANDLE hfileOut;
CRITICAL_SECTION crit;
string str;
double seconds;
int next_thread;

string paths[100] = { "C:\\Users\\bujor\\source\\repos\\input_drum_1.txt",
                      "C:\\Users\\bujor\\source\\repos\\input_drum_2.txt",
                      "C:\\Users\\bujor\\source\\repos\\input_drum_3.txt",
                      "C:\\Users\\bujor\\source\\repos\\input_drum_4.txt",
                      "C:\\Users\\bujor\\source\\repos\\input_drum_5.txt" 
                    };

int moments_rcm[100][2];

int main() {
    if (!(hfile = CreateFile("C:\\Users\\bujor\\source\\repos\\input_intersectie.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    if (!(hfileOut = CreateFile("C:\\Users\\bujor\\source\\repos\\output.txt", FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    DWORD byte = 0;
    char s[300];
    if (!ReadFile(hfile, s, 300, &byte, NULL))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    s[299] = '\0';

    hMapFileIn = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, TEXT("Local\\input_controlor_intersectie"));
    if (hMapFileIn == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }
    pBufIn = (LPTSTR)MapViewOfFile(hMapFileIn, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    if (pBufIn == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
        CloseHandle(hMapFileIn);
        return 1;
    }
    hMapFileOut = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, TEXT("Local\\output_controlor_intersectie"));
    if (hMapFileOut == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }
    pBufOut = (LPTSTR)MapViewOfFile(hMapFileIn, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    if (pBufOut == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
        CloseHandle(hMapFileIn);
        return 1;
    }

    hEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("EVENT_CCM"));

    if (hEvent == NULL)
    {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return 0;
    }
    int unities_ccm = 0;
    rowToMutexFile = CreateMutex(NULL, FALSE, NULL);

    InitializeCriticalSection(&crit);

    while (unities_ccm < ITERATIONS) {

        auxpBufIn = pBufIn;

        if (unities_ccm == 0)
        {
            for (int i = 0; i < WORKER_THREADS_COUNT; i++)
            {
                anonMutex[i] = CreateMutex(NULL, FALSE, NULL);
                int* count = new int(i);
                hThreadArray[i] = CreateThread(NULL, 0, RCM, count, 0, &dwThreadIdArray[i]);

                if (hThreadArray[i] == NULL)
                {
                    MyErrorHandler(LPSTR("CreateThread"));
                    ExitProcess(2);
                }
            }
        }

        Sleep(10);

        if (WAIT_TIMEOUT == WaitForMultipleObjects(WORKER_THREADS_COUNT, anonMutex, TRUE, 1000))
        {
            MyErrorHandler(LPSTR("WaitForMultipleObjects"));
            time_out++;
            if (time_out == 3)
                ExitProcess(3);
        }

        bool train = false;
        if (s[unities_ccm * 3] == '1')
        {
            train = true;
            string str = "La momentul " + to_string(unities_ccm) + " prin intersectie trece trenul. \n";
            if (!WriteFile(hfileOut, str.c_str(), strlen(str.c_str()), &byte, NULL))
            {
                printf("Error: %d", GetLastError());
                return 0;
            }
        }

        CopyMemory(moments_rcm, (PVOID)pBufIn, sizeof(int)* WORKER_THREADS_COUNT*2);

        for (int i = 0; i < WORKER_THREADS_COUNT; i++)
        {
            for (int j = i + 1; j < WORKER_THREADS_COUNT; j++)
                if (moments_rcm[i][1] >= moments_rcm[j][1])
                {
                    swap(moments_rcm[i][1], moments_rcm[j][1]);
                    swap(moments_rcm[i][0], moments_rcm[j][0]);
                }
        }
        int nr_vehicles = 0;
        for (int i = 0; i < WORKER_THREADS_COUNT; i++)
            if (moments_rcm[i][1] != 0)
                nr_vehicles++;
        if(train)
            nr_vehicles++;

        double times[100][2];
        if(nr_vehicles!=0)
            seconds = 60.0 / nr_vehicles;
        double time;
        
        if (train)
            time = seconds;
        else
            time = 0;

        for(int i = WORKER_THREADS_COUNT - 1 ; i >=0 ; i--)
            if (moments_rcm[i][1] != 0)
            {
                times[WORKER_THREADS_COUNT-i-1][0] = moments_rcm[i][0];
                times[WORKER_THREADS_COUNT-i-1][1] = time;
                time += seconds;
            }
            else
            {
                times[WORKER_THREADS_COUNT-i-1][0] = moments_rcm[i][0];
                times[WORKER_THREADS_COUNT-i-1][1] = -1;
            }
        next_thread = times[0][0];
        Sleep(10);


        if (nr_vehicles == 0)
            next_thread = -1;
        CopyMemory((PVOID)pBufOut,times,sizeof(times));

        for (auto mutex : anonMutex)
            ReleaseMutex(mutex);

        SetEvent(hEvent);
        ResetEvent(hEvent); 
        Sleep(10);

        if (WAIT_TIMEOUT == WaitForMultipleObjects(WORKER_THREADS_COUNT, anonMutex, TRUE, 1000))
        {
            MyErrorHandler(LPSTR("WaitForMultipleObjects"));
            time_out++;
            if (time_out == 3)
                ExitProcess(3);
        }
        for (auto mutex : anonMutex)
            ReleaseMutex(mutex);
        SetEvent(hEvent);
        ResetEvent(hEvent);
        unities_ccm++;
    }

    if (WAIT_FAILED == WaitForMultipleObjects(WORKER_THREADS_COUNT, hThreadArray, TRUE, INFINITE)){
        MyErrorHandler(LPSTR("WaitForMultipleObjects"));
        ExitProcess(3);
    }
    DeleteCriticalSection(&crit);

    DWORD dwWorkerExitCode, dwMainExitCode = 0;
    BOOL bSomeWorkerHasFailed = FALSE;

    for (int i = 0; i < WORKER_THREADS_COUNT; i++)
    {
        if (0 == GetExitCodeThread(hThreadArray[i], &dwWorkerExitCode))
        {   
            MyErrorHandler(LPSTR("GetExitCodeThread"));
            ExitProcess(4);
        }
        if (dwWorkerExitCode == 1)
        {
            _ftprintf(stderr, TEXT("A aparut o eroare legata de streamul Stderr in al %d-lea worker thread.\n"), i);
            bSomeWorkerHasFailed = TRUE; dwMainExitCode = 5;
        }
        CloseHandle(hThreadArray[i]);
    }
    if (bSomeWorkerHasFailed)  return dwMainExitCode;
    return 0;
}


DWORD WINAPI RCM(LPVOID nrThread) {
    int nr_Thread = *(int*)nrThread;

    HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
    if (hStderr == INVALID_HANDLE_VALUE)
        return 1;
    
    HANDLE hfile;
    if (!(hfile = CreateFile((paths[nr_Thread]).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    char s[300];
    DWORD byte = 0;
    if (!ReadFile(hfile, s, 300, &byte, NULL))
    {
        printf("Error: %d", GetLastError());
        return 0;
    }
    s[299] = '\0';
    for (int k = 0; k < ITERATIONS; k++)
    {
            WaitForSingleObject(anonMutex[nr_Thread], INFINITE); 
            EnterCriticalSection(&crit);
            int car_rcm[2];
            car_rcm[0] = nr_Thread;
            car_rcm[1] = 0;
            if (s[k*3] == '1')
                car_rcm[1] = 1;
            else if (s[k * 3] == '2')
                car_rcm[1] = 2;
            CopyMemory((PVOID)auxpBufIn, car_rcm, sizeof(car_rcm));
            auxpBufIn += 8;
            LeaveCriticalSection(&crit);
            ReleaseMutex(anonMutex[nr_Thread]);
            Sleep(10);
            WaitForSingleObject(hEvent, INFINITE);


            WaitForSingleObject(anonMutex[nr_Thread], INFINITE);
            double times[100][2], seconds;
            CopyMemory(times, (PVOID)pBufOut, sizeof(double) * WORKER_THREADS_COUNT * 2);
            while (true)
            {
                WaitForSingleObject(rowToMutexFile, INFINITE);
                if (nr_Thread == next_thread || next_thread==-1)
                    break;
                else
                    ReleaseMutex(rowToMutexFile);
            }
            if(next_thread!=-1)
                for (int i = 0; i <= WORKER_THREADS_COUNT - 1; i++)
                {
                    if (nr_Thread == times[i][0] && times[i][1] != -1)
                    {
                        seconds = times[i][1];
                        str = "La momentul " + to_string(k) + ", la secunda " + to_string(seconds) + " trece prin intersectie autovehiculul de pe drumul " + to_string(nr_Thread) + "\n";
                        if (!WriteFile(hfileOut, str.c_str(), strlen(str.c_str()), &byte, NULL))
                        {
                            printf("Error: %d", GetLastError());
                        }
                    }
                    if (times[i][0] == nr_Thread)
                    {
                        if (i != WORKER_THREADS_COUNT - 1)
                            next_thread = times[i + 1][0];
                        else
                            next_thread = -1;
                    }
                }
            ReleaseMutex(rowToMutexFile);
            ReleaseMutex(anonMutex[nr_Thread]);
            WaitForSingleObject(hEvent, INFINITE);
    }
    return 0;
}

void MyErrorHandler(LPTSTR lpszFunction){
    DWORD dw = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf,0,NULL);
    _ftprintf(stderr, TEXT("The WinAPI call %s failed with error %d: %s"), lpszFunction, dw, (LPTSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
}