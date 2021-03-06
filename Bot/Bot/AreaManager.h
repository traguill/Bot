#ifndef __AREA_MANAGER_H__
#define __AREA_MANAGER_H__

#include "Point.h"
#include <string>
#include <map>
#include <windows.h>
#include "Data.h"

using namespace std;

#define AREA_FILENAME "areas.json"

struct Area
{
	Point<int> left_top;
	Point<int> bottom_right;
	string name;

	Area();
	Area(const string& name, int left, int top, int bottom, int right);
};

class AreaManager
{
public:
	AreaManager();
	~AreaManager();

	void Init();

	void Update();

	bool ExistsArea(const string& name)const; //Check if there is an area with the given name
	bool ExistsArea(const string& name, Area& result)const;
	Area* FindArea(const string& name)const;
	bool CreateArea(const string& name, int left, int top, int bottom, int right);
	bool RemoveArea(const string& name);
	bool Rename(const string& old_name, const string& new_name);
	bool EditArea(const string& name, int left, int top, int bottom, int right);
	bool MakeAreaVisible(const string& name);

	bool AreaEditingUpdate(); //Returns true when the edition is done

	void GetLastAreaSize(int& top, int& left, int& bottom, int& right);

	void PrintAllAreas()const;

	bool GetRndPointArea(const Area* area, Point<int>& result)const;


private:

	void LoadAreas();
	void SaveAreas();
	//Utility
	void SerializeArea(Data& file, const Area* area)const;


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
