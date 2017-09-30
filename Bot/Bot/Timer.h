#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	void Init();

	float Update();

private:

	__int64 frequency = 0;
	float ticks_per_ms = 0.0f;
	__int64 start_time = 0;
	float delta_time = 0.0f;
};

extern Timer* timer;
#endif // !__TIMER_H__

