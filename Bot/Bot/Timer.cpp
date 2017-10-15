#include "Timer.h"

#include <windows.h>

Timer::Timer()
{}

Timer::~Timer()
{
}

void Timer::Init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	ticks_per_ms = (float)(frequency / 1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);
}

float Timer::Update()
{
	INT64 current_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);

	float difference = (float)(current_time - start_time);
	delta_time = (difference / ticks_per_ms) / 1000.0f;

	start_time = current_time;

	return delta_time;
}
