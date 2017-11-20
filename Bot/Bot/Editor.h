#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Point.h"

class AreaManager;
class MouseController;
class BTManager;

enum EDITOR_STATE
{
	SLEEP,
	CREATING_AREA,
	MOUSE_MOVING,
	BT_RUNNING,
	LOG_TRACKING
};

class Editor
{
public:
	Editor();
	~Editor();

	void Init();

	bool Update();

	bool ChangeState(EDITOR_STATE new_state);

public:
	AreaManager* area_manager = nullptr; //This should not be here.
	MouseController* mouse_controller = nullptr;
	BTManager* bt_manager = nullptr;

private:
	EDITOR_STATE state = SLEEP;
};




#endif