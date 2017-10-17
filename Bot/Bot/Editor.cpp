#include "Editor.h"

#include <Windows.h>

#include "Application.h"
#include "AreaManager.h"

Editor::Editor()
{
	area_manager = new AreaManager();
}

Editor::~Editor()
{
	delete area_manager;
}

bool Editor::Update()
{

	//Change this....
	area_manager->Update();

	switch (state)
	{
	case SLEEP:
		//Do nothing
		break;
	case CREATING_AREA:
	{	
		bool ret = area_manager->AreaEditingUpdate(); 
		if (ret == true)
		{
			App->UnblockConsole();
			state = SLEEP;
		}
	}
		break;
	}
	return true;
}

bool Editor::ChangeState(EDITOR_STATE new_state)
{
	state = new_state; //In the future check if the current state is able to change to another or it has to wait. 
	return true;
}
