#include "main.h"

vector<string> highPriorityProcesses = {"chrome.exe"};

//Check if this process is in the list of processes set to be high priority
bool processIsHighPriority(string processName)
{	
	for (string highPriorityProcess : highPriorityProcesses)
	{
		if (highPriorityProcess == processName)
		{
			return true;
		}
	}

	return false;
}

void setProcessPriority(string id, CpuPriority priority)
{
	string setPriority = "16384";

	/*
	Priority:
	idle: 64 (or "idle")
	below normal: 16384 (or "below normal")
	normal: 32 (or "normal")
	above normal: 32768 (or "above normal")
	high priority: 128 (or "high priority")
	real time: 256 (or "realtime")
	*/
	switch (priority)
	{
	case PRIORITY_IDLE:
		setPriority = "64";
		break;
	case PRIORITY_BELOW_NORMAL:
		setPriority = "16384";
		break;
	case PRIORITY_NORMAL:
		setPriority = "32";
		break;
	case PRIORITY_ABOVE_NORAML:
		setPriority = "32768";
		break;
	case PRIORITY_HIGH:
		setPriority = "128";
		break;
	case PRIORITY_REAL_TIME:
		setPriority = "256";
		break;
	default:
		break;
	}

	string command = "wmic process where 'ProcessID = " + id + "' setpriority " + setPriority;
	WinExec(command.c_str(), SW_HIDE);
}

void setProcessPrioritiesViaSnapshot()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return;

	// Retrieve information about the first process, exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
		return;

	// Iterate snapshot of processes
	do
	{	
		CpuPriority setPriority = (processIsHighPriority(pe32.szExeFile) 
			? PRIORITY_HIGH : PRIORITY_BELOW_NORMAL);

		setProcessPriority(std::to_string(pe32.th32ProcessID), setPriority);
			
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return;
}

int main()
{
	while (true)
	{
		setProcessPrioritiesViaSnapshot();
	}	
}