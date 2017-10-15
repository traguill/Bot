#include "Editor.h"

#include <Windows.h>

#include "Application.h"
#include "Input.h"

Editor::Editor()
{
}

Editor::~Editor()
{
}

bool Editor::Update()
{
	if (App->input->GetMouseButton(0) == BUTTON_STATE::DOWN)
	{
		down_pos = App->input->GetMousePosition();
		printf("Mouse Down - x:%i y:%i", down_pos.x, down_pos.y);
	}

	if (App->input->GetMouseButton(0) == BUTTON_STATE::REPEAT)
	{
		//printf("repeat");
	}

	if (App->input->GetMouseButton(0) == BUTTON_STATE::UP)
	{
		Point<int> up_pos = App->input->GetMousePosition();

		int left, right, up, down;

		if (up_pos.x > down_pos.x)
		{
			right = up_pos.x;
			left = down_pos.x;
		}
		else
		{
			right = down_pos.x;
			left = up_pos.x;
		}

		if (up_pos.y > down_pos.y)
		{
			down = up_pos.y;
			up = down_pos.y;
		}
		else
		{
			down = down_pos.y;
			up = up_pos.y;
		}

		HDC const hdc = GetDC(0);
		Rectangle(hdc, left, up, right, down);
	}
	return true;
}
