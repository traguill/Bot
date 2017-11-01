#include "MouseEmulator.h"

#include "Timer.h"

#include <windows.h>
#include <iostream>
#include "ConsoleMsgs.h"
#include "Application.h"

#include "Input.h"

MouseEmulator::MouseEmulator()
{
	screen_size.x = GetSystemMetrics(SM_CXSCREEN);
	screen_size.y = GetSystemMetrics(SM_CYSCREEN);
}


MouseEmulator::~MouseEmulator()
{}

void MouseEmulator::AddPoint(int x, int y)
{
	float x_conv = (float)x / (float)screen_size.x * 65535.0f;//Mouse coordinates screen conversion
	float y_conv = (float)y / (float)screen_size.y * 65535.0f;
	points.push_back(Point<int>(x_conv, y_conv));
}

void MouseEmulator::PrintPoints()
{
	for (vector<Point<int>>::iterator p = points.begin(); p != points.end(); ++p)
		MSG_INFO("Click x: %i y: %i", (*p).x, (*p).y);
}

void MouseEmulator::InitMovement(float sec_delay)
{
	countdown_init = sec_delay;
	speed = 800;
	dst_threshold = 30;
	can_start = false;
}

bool MouseEmulator::Move(float dt)
{
	//Can start?
	if (!can_start)
	{
		countdown_init -= dt;
		if (countdown_init < 0)
		{
			can_start = true;
			countdown_init = 0.0f;

		}
		else
			return false;
	}

	POINT cursor_pos;
	GetCursorPos(&cursor_pos);
	current_mouse_position.x = cursor_pos.x;
	current_mouse_position.y = cursor_pos.y;

	ScreenToMouseUnits(current_mouse_position.x, current_mouse_position.y);
	
	if (!has_dst)
		if (ComputeDst())
			return true; //End

	//Calculate the new step
	if (RecomputeDst() == true)
		return false;

	Point<float> displacement = step * dt;

	displacement += accumulated_step;

	accumulated_step.x = displacement.x - floor(displacement.x);
	displacement.x = floor(displacement.x);

	accumulated_step.y = displacement.y - floor(displacement.y);
	displacement.y = floor(displacement.y);

	Point<int> final_pos;
	final_pos.x = current_mouse_position.x + (int)displacement.x;
	final_pos.y = current_mouse_position.y + (int)displacement.y;


	INPUT input = {0};
	input.type = INPUT_MOUSE;
	input.mi.dx = displacement.x;
	input.mi.dy = displacement.y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.mouseData = 0;
	input.mi.dwExtraInfo = NULL;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));

	return false;
}

void MouseEmulator::Clear()
{
	step.x = step.y = 0.0f;
	dst.x = dst.y = 0;
	accumulated_step.x = accumulated_step.y = 0.0f;
	has_dst = false;
	point_id = 0;
	points.clear();
}

void MouseEmulator::InitClick(float sec_delay)
{
	mouse_down = false;
	mouse_start = false;
	countdown_mouse = sec_delay;
}

bool MouseEmulator::Clicking()
{
	if (mouse_start == false)
	{
		countdown_mouse -= App->dt;
		if (countdown_mouse <= 0.0f)
		{
			mouse_start = true;
			countdown_mouse = 0.1f;
			LeftClick();
			return false;
		}
		else
			return false;
	}

	countdown_mouse -= App->dt;
	if (countdown_mouse <= 0.0f)
	{
		LeftClickUp();
		return true;
	}
	return false;
}

bool MouseEmulator::ComputeDst()
{
	if (point_id == points.size())
		return true;

	dst = points[point_id];

	has_dst = true;

	return false;
}

bool MouseEmulator::RecomputeDst()
{
	if (point_id == points.size())
	{
		return true;
	}

	if (current_mouse_position.DistanceTo(dst) < dst_threshold)
	{
		has_dst = false;
		accumulated_step.x = 0;
		accumulated_step.y = 0;
		++point_id;
		return true;
	}

	Point<int> direction = points[point_id] - current_mouse_position;
	float length = direction.Lengthf();
	step.x = ((float)direction.x / length) * speed;
	step.y = ((float)direction.y / length) * speed;

	return false;
}

void MouseEmulator::LeftClick()
{
	INPUT input = { 0 };

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));
}

void MouseEmulator::LeftClickUp()
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void MouseEmulator::ScreenToMouseUnits(int & x, int & y) const
{
	x = (float)x / (float)screen_size.x * MOUSE_SCREEN_SIZE;
	y = (float)y / (float)screen_size.y * MOUSE_SCREEN_SIZE;
}
