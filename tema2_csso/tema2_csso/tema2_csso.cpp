#include <iostream>
#include <Windows.h>
#include<t2embapi.h>
#include <TlHelp32.h>

using namespace std;

int List_Processes() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cout << "CreateToolhelp32Snapshot (of processes)";
        return 0;
    }
    if (!Process32First(hProcessSnap, &pe32))
    {
        cout<<"Process32First"; 
        CloseHandle(hProcessSnap);          
        return 0;
    }
    while (Process32Next(hProcessSnap, &pe32))
    {
        cout << "PPID is " << pe32.th32ParentProcessID << " PID is " << pe32.th32ProcessID << " Name is " << pe32.szExeFile<<"\n";
    }
    CloseHandle(hProcessSnap);
    return TRUE;
}

int Terminate_process_pid(int pid)
{
    HANDLE pHandle;
    if (!(pHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid)))
    {
        cout << "Open process error";
        return 0;
    }
    if (!TerminateProcess(pHandle, 0))
    {
        cout << "Terminate process error";
        return 0;
    }
    return 1;
}

int List_module(int pid)
{
    HANDLE hProcessSnap;
    MODULEENTRY32 me32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    me32.dwSize = sizeof(MODULEENTRY32);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cout << "CreateToolhelp32Snapshot (of processes)";
        return 0;
    }
    if (!Module32First(hProcessSnap, &me32))
    {
        cout << "Module32First error";
        CloseHandle(hProcessSnap);
        return 0;
    }
    while (Module32Next(hProcessSnap, &me32))
    {
        cout << "Name is " << me32.szModule << endl;
    }
    CloseHandle(hProcessSnap);
    return 1;
}

int suspendProcess(int pid)
{
    HANDLE hProcessSnap;
    THREADENTRY32 te32;
    HANDLE hThread;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
    te32.dwSize = sizeof(THREADENTRY32);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cout << "Snapshot Error:" << GetLastError() << endl;
        return 0;
    }
    if (!Thread32First(hProcessSnap, &te32))
    {
        cout << "Thread first Error:" << GetLastError() << endl;
        return 0;
    }
    if (te32.th32OwnerProcessID == pid )
    {
        if (!(hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID)))
        {
            cout << "Open Thread Error: " << GetLastError() << endl;
            return 0;
        }
        SuspendThread(hThread);
    }

    while (Thread32Next(hProcessSnap, &te32))
    {
        if (te32.th32OwnerProcessID == pid)
        {
            if (!(hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID)))
            {
                cout << "Open Thread Error: " << GetLastError() << endl;
                return 0;
            }
            SuspendThread(hThread);
        }
    }
    CloseHandle(hProcessSnap);
    return 1;
}

int startExe(string path)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    if (!CreateProcess(path.c_str(), NULL, NULL, NULL, FALSE, NULL, 0, 0, &startupInfo, &processInformation))
    {
        cout << "Create process error";
        return 0;
    }
    WaitForSingleObject(processInformation.hProcess, INFINITE);
    HANDLE pToken;
    if (!OpenProcessToken(processInformation.hProcess, TOKEN_ADJUST_PRIVILEGES, &pToken))
    {
        cout << "Open process error";
        return 0;
    }
    TOKEN_PRIVILEGES tokenPrivi;
    LUID luid;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
    {
        cout << "Lookup privilege error";
        return 0;
    }
    tokenPrivi.PrivilegeCount = 1,tokenPrivi.Privileges[0].Luid = luid,tokenPrivi.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(pToken, FALSE, &tokenPrivi, 0, NULL, NULL))
    {
        cout << "Adjust privilege error";
        return 0;
    }
    CloseHandle(pToken);
    return 1;
}

int process1(string path,string parameter)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    if (!CreateProcess(path.c_str(), (LPSTR)(parameter.c_str()), NULL, NULL, FALSE, NULL, 0, 0, &startupInfo, &processInformation))
    {
        cout << "Create process error";
        return 0;
    }
    return 1;
}

int process2(string path)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    if (!CreateProcess(path.c_str(), NULL, NULL, NULL, FALSE, NULL, 0, 0, &startupInfo, &processInformation))
    {
        cout << "Create process error";
        return 0;
    }
    return 1;
}

int main()
{
    cout << "Please choose:\n1 for list processes\n2 for terminate process pid\n3 for list all modules in process\n4 for suspend process\n5 for start an exe\n6 for nr of consonant,vowel and figure\n";
    int choose;
    cin >> choose;
    if (choose == 1)
        List_Processes();
    else if (choose == 2)
    {
        int pid;
        cout << "PID: ";
        cin >> pid;
        Terminate_process_pid(pid);
    }
    else if (choose == 3)
    {
        int pid;
        cout << "PID: ";
        cin >> pid;
        List_module(pid);
    }
    else if (choose == 4)
    {
        int pid;
        cout << "PID: ";
        cin >> pid;
        suspendProcess(pid);
    }
    else if (choose == 5)
    {
        string path;
        cout << "PATH: ";
        cin >> path;
        startExe(path);
    }
    else if (choose == 6)
    {
        string path;
        cout << "PATH: ";
        cin >> path;
        process1("C:\\Users\\bujor\\source\\repos\\process1\\x64\\Debug\\process1.exe",path);
        process2("C:\\Users\\bujor\\source\\repos\\process2\\Debug\\process2.exe");
    }
    return 0;
}

