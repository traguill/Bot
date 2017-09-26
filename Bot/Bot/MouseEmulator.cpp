#include "MouseEmulator.h"

#include <windows.h>
#include <iostream>

MouseEmulator::MouseEmulator()
{}


MouseEmulator::~MouseEmulator()
{}

void MouseEmulator::AddPoint(int x, int y)
{
	points.push_back(Point<int>(x, y));
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

	SetCursorPos(final_pos.x, final_pos.y);

	//Check dst
	if (final_pos.DistanceTo(dst) < dst_threshold)
		has_dst = false;

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
