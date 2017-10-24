#include "Editor.h"

#include <Windows.h>

#include "Application.h"
#include "AreaManager.h"
#include "MouseController.h"
#include "ConsoleMsgs.h"
#include "BlackBoard.h"

Editor::Editor()
{
	area_manager = new AreaManager();
	mouse_controller = new MouseController();
	black_board = new BlackBoard();
}

Editor::~Editor()
{
	delete black_board;
	delete mouse_controller;
	delete area_manager;
}

void Editor::Init()
{
	area_manager->Init();
	black_board->Init();
}

bool Editor::Update()
{

	//Change this....
	area_manager->Update(); //To make it visible

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
	case MOUSE_MOVING:
	{
		bool ret = mouse_controller->Update();
		if (ret == true)
		{
			MSG_INFO("Move finished");
			state = SLEEP; //Done
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
