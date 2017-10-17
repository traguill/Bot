#ifndef __AREA_MANAGER_H__
#define __AREA_MANAGER_H__

#include "Point.h"
#include <string>
#include <map>
#include <windows.h>

using namespace std;

struct Area
{
	Point<int> left_top;
	Point<int> bottom_right;
	string name;

	Area(const string& name, int left, int top, int bottom, int right);
};

class AreaManager
{
public:
	AreaManager();
	~AreaManager();

	void Update();

	bool ExistsArea(const string& name)const; //Check if there is an area with the given name
	bool CreateArea(const string& name, int left, int top, int bottom, int right);
	bool RemoveArea(const string& name);
	bool Rename(const string& old_name, const string& new_name);
	bool EditArea(const string& name, int left, int top, int bottom, int right);

	bool AreaEditingUpdate(); //Returns true when the edition is done

	void GetLastAreaSize(int& top, int& left, int& bottom, int& right);

	void PrintAllAreas()const;

public:

	bool last_area_visible = false;

private:
	map<string, Area*> area_list;

	HDC hdc;

	Point<int> down_pos; //Position of the mouse when the click is made to create an area
	int left = -1; //Last area created positions
	int right = -1;
	int top = -1;
	int bottom = -1;
};
#endif
