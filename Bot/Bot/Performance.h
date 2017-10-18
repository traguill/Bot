#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

//Windows performance https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

#include <TCHAR.h>
#include <Pdh.h>

class Performance
{
public:
	Performance();
	~Performance();

private:

	void InitCPU();
	void GetCPUValue();

private:

	//CPU
	static PDH_HQUERY cpu_query;
	static PDH_HCOUNTER cpu_total;
	double cpu_value = 0;

};

#endif // !__PERFORMANCE_H__
