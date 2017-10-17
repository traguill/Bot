#include "Commands.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "Editor.h"
#include "AreaManager.h"

#include <iostream>

void CreateArea(const vector<string>* args)
{
	string input;
	MSG_INFO("Area Name: ");
	getline(cin, input);
	MSG_INFO("Make a selection of the area");

	//Request app to change to area editing
	App->editor->ChangeState(EDITOR_STATE::CREATING_AREA);
	App->BlockConsole();

	App->editor->area_manager->last_area_visible = true;
	MSG_INFO("Area: %s with position bla bla", input.data());

	bool completed = false;

	while (!completed)
	{
		completed = true;

		MSG_INFO("Save(s) - Repeat selection(r) - Discard(d)");
		getline(cin, input);

		if (input == "s")
		{
			//Save
		}
		else if (input == "r")
		{
			//Make the selection again
		}
		else if (input == "d")
		{
			//exit without doing anything
		}
		else
		{
			MSG_WARNING("Invalid option");
			completed = false;
		}
	}
	

}