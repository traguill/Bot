#include "MouseRoutine.h"

#include <iostream>

MouseRoutine::MouseRoutine()
{
}

MouseRoutine::~MouseRoutine()
{
}

void MouseRoutine::SetStartDelay(float seconds)
{
	start_delay = seconds;
}

void MouseRoutine::AddAction(int pos_x, int pos_y, float speed, bool click, float sleep)
{
	MouseAction action(pos_x, pos_y, speed, click, sleep);

	actions.push_back(action);
}

void MouseRoutine::PrintRoutine(int id) const
{
	if (id == -1)
	{
		std::cout << "Start delay: " << start_delay << "\n";
		for(int i = 0; i < actions.size(); ++i)
			std::cout << id << "- Position(" << actions[i].x << "," << actions[i].y << ") Speed: " << actions[i].speed << " Click: " << actions[i].click << " Sleep: " << actions[i].sleep << "\n";
		return;
	}

	if (id >= actions.size())
	{
		std::cout << "[Error]: Invalid routine id. The action cannot be printed. Id: " << id << ". Routine size: " << actions.size() << "\n";
		return;
	}

	std::cout << id << "- Position(" << actions[id].x << "," << actions[id].y << ") Speed: " << actions[id].speed << " Click: " << actions[id].click << " Sleep: " << actions[id].sleep << "\n";
}

MouseAction::MouseAction() : x(0), y(0), speed(0.0F), click(false), sleep(0.0f)
{
}

MouseAction::MouseAction(int x, int y, float speed, bool click, float sleep) : x(x), y(y), speed(speed), click(click), sleep(sleep)
{
}
