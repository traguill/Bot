#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

//Windows performance https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

#include <TCHAR.h>
#include <Pdh.h>

#pragma comment (lib, "pdh.lib")

#define BYTETOMEGABYTE 1048576

class Performance
{
public:
	Performance();
	~Performance();

	void Init();

private:

	//Virtual memory
	void InitVirtualMemory();

	//CPU
	void InitCPU();
	void GetCPUValue();

private:

	//Virtual memory
	DWORDLONG total_virtual_mem;

	//CPU
	PDH_HQUERY cpu_query;
	PDH_HCOUNTER cpu_total;
	double cpu_value = 0;

};

#endif // !__PERFORMANCE_H__
