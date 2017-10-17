#include "Commands.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "Editor.h"
#include "AreaManager.h"

#include <iostream>

void ClearConsoleScreen(const vector<string>* args)
{
	system("cls");
}

void CreateArea(const vector<string>* args)
{
	string name;
	MSG_INFO("Area Name: ");
	getline(cin, name);
	
	MSG_INFO("Make a selection of the area");

	bool repeat = true; //Repeat the selection

	while (repeat)
	{
		repeat = false;
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
				repeat = true;
				App->editor->area_manager->last_area_visible = false;
				MSG_INFO("Repeat the selection of the area.");
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

	App->editor->area_manager->last_area_visible = false; //Hide area
}

void ListAreas(const vector<string>* args)
{
	App->editor->area_manager->PrintAllAreas();
}

void RemoveArea(const vector<string>* args)
{
	if (args == nullptr)
	{
		MSG_ERROR("Nullptr passed to RemoveArea() method."); //Should never happen...? Leave it just in case.
		return;
	}

	if (args->size() == 0)
	{
		MSG_ERROR("Error: option -r requires a value");
		MSG_INFO("Usage: area -r <area_to_remove>");
		return;
	}

	string area = args->front();

	bool ret = App->editor->area_manager->RemoveArea(area);

	if (ret)
		MSG_INFO("Area %s has been removed successfully", area.data());
	else
		MSG_ERROR("Area %s has not been removed.", area.data());
	
}

void EditArea(const vector<string>* args)
{
	if (args == nullptr)
	{
		MSG_ERROR("Nullptr passed to EditArea() method.");
		return;
	}

	if (args->size() == 0)
	{
		MSG_ERROR("Error: option -e requires a value");
		MSG_INFO("Usage: area -e rename <area_to_rename> <new_name> or area -e space <area_to_edit>");
		return;
	}

	string value1 = args->front();

	//value1 = rename -> change the name | value1 = space -> edit the area space
	if (value1.compare("rename") == 0)
	{
		if (args->size() != 3) //Error
		{
			MSG_ERROR("Rename invalid parameters. Usage: 'area -e rename <area_to_rename> <new_name>'");
		}
		else //Rename
		{
			string old_name = (*args)[1];
			string new_name = (*args)[2];
			bool ret = App->editor->area_manager->Rename(old_name, new_name);
			if (ret)
				MSG_INFO("Area %s has been changed to area %s", old_name.data(), new_name.data());
			else
				MSG_ERROR("Rename failed. An error occurred");
		}

	}
	else if (value1.compare("space") == 0) //Redefine the area
	{
		if (args->size() != 2) //Error
		{
			MSG_ERROR("Space invalid parameters. Usage: 'area -e space <area_to_edit>'");
		}
		else //Edit space
		{
			string area_name = (*args)[1];
			RedefineAreaSpace(area_name);
		}
	}
	else
	{
		MSG_ERROR("Invalid area -e parameter. Accepted: 'area -e rename <area_to_rename> <new_name>' 'area -e space <area_to_edit>'");
	}

}

void RedefineAreaSpace(const string& area_name)
{

	bool exists = App->editor->area_manager->ExistsArea(area_name);
	if (!exists)
	{
		MSG_ERROR("Area %s does not exist.", area_name.data());
		return;
	}

	MSG_INFO("Make a selection of the area");

	bool repeat = true; //Repeat the selection

	while (repeat)
	{
		repeat = false;
		//Request app to change to area editing
		App->editor->ChangeState(EDITOR_STATE::CREATING_AREA);
		App->BlockConsole();

		//Show rectangle
		App->editor->area_manager->last_area_visible = true; //Change this for a function. The rectangle drawn makes the program slow (issue)

		int top, bottom, right, left;
		App->editor->area_manager->GetLastAreaSize(top, left, bottom, right);
		MSG_INFO("Area: %s with top-left: %i, %i bottom-right: %i %i", area_name.data(), top, left, bottom, right);

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
				completed = App->editor->area_manager->EditArea(area_name, left, top, bottom, right);
				if (!completed)
					MSG_ERROR("Area %s has not been edited.", area_name.data());
			}
			else if (input == "r")
			{
				//Make the selection again
				repeat = true;
				App->editor->area_manager->last_area_visible = false;
				MSG_INFO("Repeat the selection of the area.");
			}
			else if (input == "d")
			{
				//exit without doing anything
				MSG_INFO("Changes of area: %s have been discarted", area_name.data());
			}
			else
			{
				MSG_WARNING("Invalid option");
				completed = false;
			}
		}
	}

	App->editor->area_manager->last_area_visible = false; //Hide area
}
