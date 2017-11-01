#ifndef __MOUSECONTROLLER_H__
#define __MOUSECONTROLLER_H__

#include "Point.h"

class MouseEmulator;

class MouseController
{
public:
	MouseController();
	~MouseController();

	bool Update();

	void GoTo(const Point<int>& dst, float delay);

private:


};
#endif // !__MOUSECONTROLLER_H__

