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

	__int64 frequency;
	float ticks_per_ms;
	__int64 start_time;
	float delta_time;

};
#endif // !__TIMER_H__

