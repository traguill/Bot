#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Point.h"

class AreaManager;

enum EDITOR_STATE
{
	SLEEP,
	CREATING_AREA
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

private:
	EDITOR_STATE state = SLEEP;
};




#endif