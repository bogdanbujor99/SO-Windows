#include <iostream>
#include <windows.h>

int main()
{
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(STARTUPINFO);
    CreateProcess("C:\\Users\\bujor\\source\\repos\\P1\\x64\\Debug\\P1.exe", NULL, NULL, NULL, FALSE, NULL, 0, 0, &startInfo, &processInfo);
    CreateProcess("C:\\Users\\bujor\\source\\repos\\P2\\x64\\Debug\\P2.exe", NULL, NULL, NULL, FALSE, NULL, 0, 0, &startInfo, &processInfo);
}