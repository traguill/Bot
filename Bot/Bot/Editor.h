#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Point.h"

class Editor
{
public:
	Editor();
	~Editor();

	bool Update();



private:
	Point<int> down_pos;
};




#endif