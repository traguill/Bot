#include "AreaManager.h"
#include "ConsoleMsgs.h"

#include "Application.h"
#include "Input.h"

AreaManager::AreaManager()
{
	hdc = GetDC(0);
}

AreaManager::~AreaManager()
{
	for (auto a : area_list)
	{
		delete a.second;
	}
}

void AreaManager::Update()
{
	if (last_area_visible)
	{
		Rectangle(hdc, left, up, right, down); //This makes the computer slow. Investigate why.
	}
}

bool AreaManager::CreateArea(const string & name, int left, int top, int bottom, int right)
{
	if (area_list.find(name) != area_list.end())
	{
		MSG_ERROR("New area could not be created. Another area with the same name already exists");
		return false;
	}

	Area* area = new Area(name, left, top, bottom, right);

	pair<map<string, Area*>::iterator, bool> ret;
	ret = area_list.insert(pair<string, Area*>(name, area));

	return ret.second;
}

bool AreaManager::AreaEditingUpdate()
{
	if (App->input->GetMouseButton(0) == BUTTON_STATE::DOWN)
	{
		down_pos = App->input->GetMousePosition();
	}

	if (App->input->GetMouseButton(0) == BUTTON_STATE::UP)
	{
		Point<int> up_pos = App->input->GetMousePosition();

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

		return true;
	}

	return false;
}

Area::Area(const string & name, int left, int top, int bottom, int right) : name(name), left_top(Point<int>(left, top)), bottom_right(Point<int>(bottom, right))
{
}
