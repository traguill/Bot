#include "Performance.h"
#include "ConsoleMsgs.h"

Performance::Performance()
{
}

Performance::~Performance()
{
}

void Performance::Init()
{
	InitVirtualMemory();
	InitCPU();
	//Debug
	GetCPUValue();
	MSG_INFO("CPU: %d", cpu_value);
}

void Performance::InitVirtualMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	total_virtual_mem = memInfo.ullTotalPageFile;

	MSG_INFO("Total Virtual Memory: %d MB", total_virtual_mem/BYTETOMEGABYTE);
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
