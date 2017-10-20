#include "Performance.h"

Performance::Performance()
{
}

Performance::~Performance()
{
}

void Performance::Init()
{

}

void Performance::InitCPU()
{
	/*PdhOpenQuery(NULL, NULL, &cpu_query);
	PdhAddEnglishCounter(cpu_query, "\\Processor(_Total)\\% Processor Time", NULL, &cpu_total);
	PdhCollectQueryData(cpu_query);*/
}

void Performance::GetCPUValue()
{
	/*PDH_FMT_COUNTERVALUE counter_val;

	PdhCollectQueryData(cpu_query);
	PdhGetFormattedCounterValue(cpu_total, PDH_FMT_DOUBLE, NULL, &counter_val);
	cpu_value = counter_val.doubleValue;*/
}
