#include "Performance.h"
#include "ConsoleMsgs.h"

#include <Psapi.h>

Performance::Performance()
{
}

Performance::~Performance()
{
}

void Performance::Init()
{
	InitVirtualMemory();
	GetVirtualMemory();
	MSG_INFO("Virtual Memory used: %.2f MB", virtual_mem_used*BYTETOMEGABYTE);
	InitCPU();
	InitCPUProcess();
	GetCPUValue();
	MSG_INFO("CPU: %d", cpu_value);
	GetCPUProcessValue();
	MSG_INFO("Process CPU: %d", cpu_percentage_process);
}

void Performance::Update()
{
	system("cls");
	GetProcessVirtualMemory();
	MSG_INFO("Process Virtual Memory used: %.2f MB", virtual_mem_proc_used*BYTETOMEGABYTE);
}

void Performance::InitVirtualMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	total_virtual_mem = memInfo.ullTotalPageFile;
	MSG_INFO("Total Virtual Memory: %.2f MB", total_virtual_mem*BYTETOMEGABYTE); //DEBUG

	total_phys_mem = memInfo.ullTotalPhys;
	MSG_INFO("Total RAM: %.2f MB", total_virtual_mem*BYTETOMEGABYTE);
}

void Performance::GetVirtualMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	virtual_mem_used = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

	phys_mem_used = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	MSG_INFO("Used RAM: %.2f MB", phys_mem_used*BYTETOMEGABYTE);
}

void Performance::GetProcessVirtualMemory()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	virtual_mem_proc_used = pmc.PrivateUsage;

	phys_mem_pro_used = pmc.WorkingSetSize;
	MSG_INFO("Process Used RAM: %.2f MB", phys_mem_pro_used*BYTETOMEGABYTE);
}

void Performance::InitCPU()
{
	PdhOpenQuery(NULL, NULL, &cpu_query);
	PdhAddEnglishCounter(cpu_query, "\\Processor(_Total)\\% Processor Time", NULL, &cpu_total);
	PdhCollectQueryData(cpu_query);
}

void Performance::GetCPUValue()
{
	PDH_FMT_COUNTERVALUE counter_val;

	PdhCollectQueryData(cpu_query);
	PdhGetFormattedCounterValue(cpu_total, PDH_FMT_DOUBLE, NULL, &counter_val);
	cpu_value = counter_val.doubleValue;
}

void Performance::InitCPUProcess()
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

	MSG_INFO("Number of processors: %i", numProcessors);
}

void Performance::GetCPUProcessValue()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	cpu_percentage_process = (sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart);
	cpu_percentage_process /= (now.QuadPart - lastCPU.QuadPart);
	cpu_percentage_process /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	cpu_percentage_process *= 100;
}
