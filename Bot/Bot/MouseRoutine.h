#ifndef __MOUSEROUTINE_H__
#define __MOUSEROUTINE_H__

#include <vector>

struct MouseAction
{
	int x, y;
	float speed;
	bool click;
	float sleep;

	MouseAction();
	MouseAction(int x, int y, float speed, bool click, float sleep);
};

class MouseRoutine
{
public:
	MouseRoutine();
	~MouseRoutine();

	void SetStartDelay(float seconds);
	void AddAction(int pos_x, int pos_y, float speed, bool click, float sleep);

	//Debug
	void PrintRoutine(int id)const; //Prints the specific action of the routine. If -1 is passed, it prints the whole routine.

private:

	float start_delay = 0.0f; //Seconds to wait before starting the routine
	std::vector<MouseAction> actions;

};

#endif // !__MOUSEROUTINE_H__
