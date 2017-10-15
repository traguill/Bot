#include "Input.h"
#include <windows.h>

Input::Input()
{
	screen_size.x = GetSystemMetrics(SM_CXSCREEN);
	screen_size.y = GetSystemMetrics(SM_CYSCREEN);
}

Input::~Input()
{
}

bool Input::Update()
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	mouse_position.x = cursor_pos.x;
	mouse_position.y = cursor_pos.y;

	UpdateButtonState(mouse_lb);

	for (auto in : input_to_process)
	{
		if (in.src != nullptr)
		{
			*in.src = in.new_state;
		}
	}

	input_to_process.clear();
	return true;
}

void Input::MouseButton(int id, BUTTON_STATE state)
{
	if (id == 0) //Left
	{
		input_to_process.push_back(ButtonStateTMP(&mouse_lb, state));
	}
}

void Input::MouseMove(int x, int y)
{
	mouse_position.x = x;
	mouse_position.y = y;
}

BUTTON_STATE Input::GetMouseButton(int id) const
{
	if (id == 0)
	{
		return mouse_lb;
	}
	return BUTTON_STATE::IDLE;
}

Point<int> Input::GetMousePosition() const
{
	return mouse_position;
}

void Input::MouseToScreenUnits(int & x, int & y) const
{
	x = (float)x / MOUSE_SCREEN_SIZE * (float)screen_size.x;
	y = (float)y / MOUSE_SCREEN_SIZE * (float)screen_size.y;
}

void Input::UpdateButtonState(BUTTON_STATE & button)
{
	if (button == BUTTON_STATE::DOWN)
	{
		button = BUTTON_STATE::REPEAT;
	}
	else if (button == BUTTON_STATE::UP)
	{
		button = BUTTON_STATE::IDLE;
	}
}

ButtonStateTMP::ButtonStateTMP()
{
}

ButtonStateTMP::ButtonStateTMP(BUTTON_STATE* src, BUTTON_STATE new_state) : src(src), new_state(new_state)
{}
