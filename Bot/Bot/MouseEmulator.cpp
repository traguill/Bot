#include "MouseEmulator.h"

#include <windows.h>
#include <iostream>

MouseEmulator::MouseEmulator()
{
	screen_size.x = GetSystemMetrics(SM_CXSCREEN);
	screen_size.y = GetSystemMetrics(SM_CYSCREEN);

	cout << "Screeen : " << screen_size.x << "///" << screen_size.y << "\n";
}


MouseEmulator::~MouseEmulator()
{}

void MouseEmulator::AddPoint(int x, int y)
{
	float x_conv = (float)x / (float)screen_size.x * 65535.0f;//Mouse coordinates screen conversion
	float y_conv = (float)y / (float)screen_size.y * 65535.0f;
	points.push_back(Point<int>(x_conv, y_conv));

	/*cout << "Click x:  " << x << " y: " << y << "\n";
	cout << "Click[CONV] x:  " << x_conv << " y: " << y_conv << "\n";
	cout << "Click[RES] x:  " << points.back().x << " y: " << points.back().y << "\n";*/
}

void MouseEmulator::PrintPoints()
{
	for (vector<Point<int>>::iterator p = points.begin(); p != points.end(); ++p)
		cout << "Click x:  " << (*p).x << " y: " << (*p).y << "\n";
}

void MouseEmulator::InitMovement()
{
	step.x = step.y = 0.0f;
	dst.x = dst.y = 0;
	accumulated_step.x = accumulated_step.y = 0.0f;
	has_dst = false;
	point_id = 0;
}

bool MouseEmulator::Move(float dt)
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	current_mouse_position.x = cursor_pos.x;
	current_mouse_position.y = cursor_pos.y;

	ScreenToMouseUnits(current_mouse_position.x, current_mouse_position.y);

	if (!has_dst)
		if (ComputeDst())
			return true; //End

	//Calculate the new step
	Point<float> displacement = step * dt;

	displacement += accumulated_step;

	accumulated_step.x = displacement.x - floor(displacement.x);
	displacement.x = floor(displacement.x);

	accumulated_step.y = displacement.y - floor(displacement.y);
	displacement.y = floor(displacement.y);


	Point<int> final_pos;
	final_pos.x = current_mouse_position.x + (int)displacement.x;
	final_pos.y = current_mouse_position.y + (int)displacement.y;


	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = displacement.x;
	input.mi.dy = displacement.y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.mouseData = 0;
	input.mi.dwExtraInfo = NULL;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
	//SetCursorPos(final_pos.x, final_pos.y);

	//Check dst
	if (final_pos.DistanceTo(dst) < dst_threshold)
	{
		has_dst = false;
		LeftClick();
	}

	return false;
}

bool MouseEmulator::ComputeDst()
{
	if (point_id == points.size())
		return true;

	Point<int> direction = points[point_id] - current_mouse_position;

	float length = direction.Lengthf();
	step.x = (direction.x / length) * speed;
	step.y = (direction.y / length) * speed;

	dst = points[point_id];

	has_dst = true;

	++point_id;

	return false;
}

void MouseEmulator::LeftClick()
{
	INPUT input = { 0 };

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void MouseEmulator::ScreenToMouseUnits(int & x, int & y) const
{
	x = (float)x / (float)screen_size.x * MOUSE_SCREEN_SIZE;
	y = (float)y / (float)screen_size.y * MOUSE_SCREEN_SIZE;
}
