#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

//Windows performance https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

#include <TCHAR.h>
#include <Pdh.h>

#pragma comment (lib, "pdh.lib")

#define BYTETOMEGABYTE 0.000000953674316	

class Performance
{
public:
	Performance();
	~Performance();

	void Init();
	void Update();

private:

	//Note all Get methods refer to the current value of the item

	//Virtual memory
	void InitVirtualMemory();
	void GetVirtualMemory();
	void GetProcessVirtualMemory();

	//CPU
	void InitCPU();
	void GetCPUValue();
	void InitCPUProcess();
	void GetCPUProcessValue();

private:

	//Virtual memory
	DWORDLONG total_virtual_mem;
	DWORDLONG virtual_mem_used;
	SIZE_T virtual_mem_proc_used;

	//RAM
	DWORDLONG total_phys_mem;
	DWORDLONG phys_mem_used;
	SIZE_T phys_mem_pro_used;

	//CPU
	PDH_HQUERY cpu_query;
	PDH_HCOUNTER cpu_total;
	double cpu_value = 0;

	//CPU of this process
	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
	double cpu_percentage_process = 0;

};

#endif // !__PERFORMANCE_H__
