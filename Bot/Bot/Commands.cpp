#include "Commands.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "Editor.h"
#include "AreaManager.h"

#include <iostream>

void CreateArea(const vector<string>* args)
{
	string name;
	MSG_INFO("Area Name: ");
	getline(cin, name);
	MSG_INFO("Make a selection of the area");

	//Request app to change to area editing
	App->editor->ChangeState(EDITOR_STATE::CREATING_AREA);
	App->BlockConsole();

	//Show rectangle
	App->editor->area_manager->last_area_visible = true; //Change this for a function. The rectangle drawn makes the program slow (issue)

	int top, bottom, right, left;
	App->editor->area_manager->GetLastAreaSize(top, left, bottom, right);

	MSG_INFO("Area: %s with top-left: %i, %i bottom-right: %i %i", name.data(), top, left, bottom, right);

	bool completed = false;

	string input;
	while (!completed)
	{
		completed = true;

		MSG_INFO("Save(s) - Repeat selection(r) - Discard(d)");
		getline(cin, input);

		if (input == "s")
		{
			//Save
			completed = App->editor->area_manager->CreateArea(name, left, top, bottom, right);
			if (!completed)
				MSG_ERROR("Area %s has not been created.", name.data());
		}
		else if (input == "r")
		{
			//Make the selection again
		}
		else if (input == "d")
		{
			//exit without doing anything
			MSG_INFO("Changes of area: %s have been discarted", name.data());
		}
		else
		{
			MSG_WARNING("Invalid option");
			completed = false;
		}
	}
}

void ListAreas(const vector<string>* args)
{
	App->editor->area_manager->PrintAllAreas();
}