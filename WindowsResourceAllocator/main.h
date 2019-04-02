#pragma once

#include <windows.h>
#include "tlhelp32.h"
#include "vector"
#include "string"
#include "psapi.h"

using namespace std;

enum CpuPriority
{
	PRIORITY_IDLE,
	PRIORITY_BELOW_NORMAL,
	PRIORITY_NORMAL,
	PRIORITY_ABOVE_NORAML,
	PRIORITY_HIGH,
	PRIORITY_REAL_TIME,
};

string getProcessNameFromID(DWORD id);
bool processIsHighPriority(string processName);
void setProcessPriority(string id, CpuPriority priority);
void setProcessPriorities();
void setProcessPrioritiesViaSnapshot();