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
		Rectangle(hdc, left, top, right, bottom); //This makes the computer slow. Investigate why.
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

bool AreaManager::RemoveArea(const string & name)
{
	map<string, Area*>::iterator it = area_list.find(name);

	if (it == area_list.end())
	{
		MSG_ERROR("Area %s does not exist. Remove failed.", name.data());
		return false;
	}
	else
	{
		delete it->second;
		area_list.erase(it);
		return true;
	}

	return false;
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
			bottom = up_pos.y;
			top = down_pos.y;
		}
		else
		{
			bottom = down_pos.y;
			top = up_pos.y;
		}

		return true;
	}

	return false;
}

void AreaManager::GetLastAreaSize(int & top, int & left, int & bottom, int & right)
{
	top = this->top;
	left = this->left;
	right = this->right;
	bottom = this->bottom;
}

void AreaManager::PrintAllAreas() const
{
	if (area_list.size() == 0)
	{
		MSG_INFO("Empty. No areas created.");
		MSG_INFO("use area -c to create a new area");
		return;
	}

	MSG_INFO("Number of areas defined: %i", area_list.size());
	MSG_INFO(""); //line jump
	for (auto a : area_list)
	{
		MSG_INFO("* %s", a.first.data());
	}
}

Area::Area(const string & name, int left, int top, int bottom, int right) : name(name), left_top(Point<int>(left, top)), bottom_right(Point<int>(bottom, right))
{
}
