#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

DWORD GetPID(const char* ProcessName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);


	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!strcmp(processInfo.szExeFile, ProcessName))
	{
		CloseHandle(processesSnapshot);
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!strcmp(processInfo.szExeFile, ProcessName))
		{
			CloseHandle(processesSnapshot);
		}
	}
	CloseHandle(processesSnapshot);
	return processInfo.th32ProcessID;
}
MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!strcmp(curr.szModule, moduleName))
				{
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}
int main()
{
	unsigned long pid = GetPID("starwarsbattlefrontii.exe");
	MODULEENTRY32 module = GetModule("starwarsbattlefrontii.exe", pid);

	HANDLE phandle = OpenProcess(PROCESS_VM_READ, 0, pid);

	cout << "Attatched to process id: " << pid << endl;

	unsigned long long addr;

	for (;;)
	{
		int looking;

		ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x03A931B0), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0x30), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0x8), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0x48), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0x10), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0x238), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0x130), &addr, sizeof(addr), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)addr + 0xAE0), &looking, sizeof(looking), 0);

		while (looking == 1 && GetAsyncKeyState(VK_RBUTTON) || looking == 1 && GetAsyncKeyState(0x43)) //Can be changed to any keybind https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			break;
		}

	}
}