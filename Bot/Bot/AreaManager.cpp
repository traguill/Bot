#include "AreaManager.h"
#include "ConsoleMsgs.h"

#include "Application.h"
#include "Input.h"
#include "ModuleFileSystem.h"
#include "Random.h"

AreaManager::AreaManager()
{
	hdc = GetDC(0);
}

AreaManager::~AreaManager()
{
	SaveAreas();

	for (auto a : area_list)
	{
		delete a.second;
	}
}

void AreaManager::Init()
{
	LoadAreas();
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

bool AreaManager::ExistsArea(const string & name, Area & result) const
{
	bool ret = false;

	map<string, Area*>::const_iterator found = area_list.find(name);
	if (found == area_list.end())
		ret = false;
	else
	{
		result = *(*found).second;
		ret = true;
	}

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


void AreaManager::LoadAreas()
{
	char* buf = nullptr;
	unsigned int load_ret = App->file_system->Load("areas.json", &buf);

	if (load_ret == 0)
		MSG_WARNING("No area filesave was found or it is empty");

	Data info(buf);

	int size = info.GetArraySize("areas");
	
	for (int i = 0; i < size; ++i)
	{
		Data a_info = info.GetArray("areas", i);

		string name = a_info.GetString("name");
		Point<int> left_top = a_info.GetInt2("left_top");
		Point<int> bottom_right = a_info.GetInt2("bottom_right");
		Area* a = new Area(name, left_top.x, left_top.y, bottom_right.x, bottom_right.y);

		area_list.insert(pair<string, Area*>(name, a));
	}


	if (buf)
		delete[] buf;

	MSG_INFO("Areas file load successfully");
}

void AreaManager::SaveAreas()
{
	Data file;

	file.AppendArray("areas");

	for (auto a : area_list)
	{
		Data a_info;
		SerializeArea(a_info, a.second);
		file.AppendArrayValue(a_info);
	}

	char* buf;
	int size = file.Serialize(&buf);

	App->file_system->Save("areas.json", buf, size);

	if (buf)
		delete[] buf;
}

void AreaManager::SerializeArea(Data & file, const Area * area) const
{
	file.AppendString("name", area->name.data());
	file.AppendInt2("left_top", (const int*)&area->left_top);
	file.AppendInt2("bottom_right", (const int*)&area->bottom_right);
}

bool AreaManager::GetRndPointArea(const Area * area, Point<int>& result) const
{
	if (area == nullptr)
		return false;

	result.x = App->rnd->RandomFloat(area->left_top.x, area->bottom_right.y);
	result.y = App->rnd->RandomFloat(area->left_top.y, area->bottom_right.x);

	return true;
}

Area::Area()
{
}

Area::Area(const string & name, int left, int top, int bottom, int right) : name(name), left_top(Point<int>(left, top)), bottom_right(Point<int>(bottom, right))
{
}
