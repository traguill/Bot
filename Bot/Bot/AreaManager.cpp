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

bool AreaManager::ExistsArea(const string & name) const
{
	bool ret = false;

	if (area_list.find(name) == area_list.end())
		ret = false;
	else
		ret = true;

	return ret;
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

bool AreaManager::Rename(const string & old_name, const string & new_name)
{
	map<string, Area*>::iterator it = area_list.find(old_name);

	if (it == area_list.end())
	{
		MSG_ERROR("Area %s does not exist. Rename failed.", old_name.data());
		return false;
	}
	else
	{
		map<string, Area*>::iterator it2 = area_list.find(new_name);

		if (it2 != area_list.end())
		{
			MSG_ERROR("Can't rename area %s to %s because area %s is already on the list", old_name.data(), new_name.data(), new_name.data());
			return false;
		}

		Area* a = it->second;
		a->name = new_name.data();
		area_list.erase(it);

		pair<map<string, Area*>::iterator, bool> ret;
		ret = area_list.insert(pair<string, Area*>(new_name, a));
		if (ret.second == false)
		{
			MSG_ERROR("Area %s could not be renamed to %s. Insertion failed. The area has been removed (sry)", old_name.data(), new_name.data()); //Should never happen, but just in case
		}

		return ret.second;
	}
	return false;
}

bool AreaManager::EditArea(const string & name, int left, int top, int bottom, int right)
{
	map<string, Area*>::iterator it = area_list.find(name);

	if (it != area_list.end())
	{
		(*it).second->left_top = Point<int>(left, top);
		(*it).second->bottom_right = Point<int>(bottom, right);
		return true;
	}

	return false;
}

bool AreaManager::MakeAreaVisible(const string & name)
{
	map<string, Area*>::iterator it = area_list.find(name);

	if (it != area_list.end())
	{
		bottom = (*it).second->bottom_right.x;
		right = (*it).second->bottom_right.y;
		left = (*it).second->left_top.x;
		top = (*it).second->left_top.y;
		last_area_visible = true;
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
