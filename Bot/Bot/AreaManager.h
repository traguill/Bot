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

	bool CreateArea(const string& name, int left, int top, int bottom, int right);

	bool AreaEditingUpdate(); //Returns true when the edition is done

public:

	bool last_area_visible = false;

private:
	map<string, Area*> area_list;

	HDC hdc;

	Point<int> down_pos; //Position of the mouse when the click is made to create an area
	int left = -1; //Last area created positions
	int right = -1;
	int up = -1;
	int down = -1;
};
#endif
