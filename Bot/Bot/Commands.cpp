#include "Commands.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "Editor.h"
#include "AreaManager.h"
#include "MouseController.h"
#include "BlackBoard.h"
#include "BTManager.h"
#include "BehaviorTree.h"
#include "CardsDB.h"

#include <iostream>

bool CheckNumParameters(const vector<string>* args, int min_number, int max_number, const char * function_name, char option)
{
	if (args == nullptr)
	{
		MSG_ERROR("Nullptr passed to %s() method.", function_name); 
		return false;
	}

	if ((int)args->size() < min_number || (int)args->size() > max_number)
	{
		MSG_ERROR("Error: option -%c requires a value", option);
		return false;
	}

	return true;
}

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

void ShowArea(const vector<string>* args)
{
	if (args == nullptr)
	{
		MSG_ERROR("Nullptr passed to ShowArea() method.");
		return;
	}

	if (args->size() == 0)
	{
		MSG_ERROR("Error: option -s requires a value");
		MSG_INFO("Usage: area -s <area_to_show>");
		return;
	}

	string name = (*args)[0];
	bool ret = App->editor->area_manager->MakeAreaVisible(name);

	if (ret == false)
		MSG_ERROR("Area %s does not exist", name.data());

}

void MoveMouseToArea(const vector<string>* args)
{
	if (args == nullptr)
	{
		MSG_ERROR("Nullptr passed to MoveMouseToArea() method.");
		return;
	}

	if (args->size() == 0)
	{
		MSG_ERROR("Error: requires a value");
		MSG_INFO("Usage: mouse <area_to_move> <delay>(optional)");
		return;
	}

	string area = (*args)[0];
	float delay = (args->size() == 2) ? stof((*args)[1]) : 0.0f;

	Area destination;
	bool area_found = App->editor->area_manager->ExistsArea(area, destination);

	if (area_found == false)
	{
		MSG_ERROR("Error area %s not found", area.data());
		return;
	}

	Point<int> dst_p;
	bool ret = App->editor->area_manager->GetRndPointArea(&destination, dst_p);

	if (!ret)
	{
		MSG_ERROR("Error: couldn't find a random point in the specified area");
		return; 
	}

	App->editor->mouse_controller->GoTo(dst_p, delay);
}

void BBShow(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 0, 0, "BBShow", 's');
	if (ret == false)
		return;

	BehaviorTree* bt = App->editor->bt_manager->GetCurrentBT();

	if (bt == nullptr)
	{
		MSG_WARNING("No BehaviorTree was loaded. Use: bt -e <bt_name> to start editing a BT and its BlackBoard");
		return;
	}

	bt->bb->PrintVars();
}

void BBInsert(const vector<string>* args) //TODO: Ask current BT
{
	bool ret = CheckNumParameters(args, 3, 3,"BBInsert", 'i');

	if (ret == false)
		return;

	BehaviorTree* bt = App->editor->bt_manager->GetCurrentBT();

	if (bt == nullptr)
	{
		MSG_WARNING("No BehaviorTree was loaded. Use: bt -e <bt_name> to start editing a BT and its BlackBoard");
		return;
	}

	string bb_var_type = (*args)[0];
	string bb_var_name = (*args)[1];
	string bb_var_value = (*args)[2];

	if (bb_var_type.compare("bool") == 0) bt->bb->InsertBool(bb_var_name, (bb_var_value.compare("0") ? false : true));
	else if (bb_var_type.compare("int") == 0) bt->bb->InsertInt(bb_var_name, stoi(bb_var_value.data()));
	else if (bb_var_type.compare("float") == 0) bt->bb->InsertFloat(bb_var_name, stof(bb_var_value.data()));
	else if (bb_var_type.compare("string") == 0) bt->bb->InsertString(bb_var_name, bb_var_value);
	else if (bb_var_type.compare("area") == 0) bt->bb->InsertArea(bb_var_name, bb_var_value);

	//TODO: vector
}

void BTGoToNode(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 1, 1, "BTGoToNode", '0');
	if (ret == false)
		return;

	BehaviorTree* bt = App->editor->bt_manager->GetCurrentBT();
	if (bt == nullptr)
	{
		MSG_WARNING("Select a BT to edit. Use bt -e <bt_name>. Use bt -s to see the BT available");
		return;
	}

	string uid_s = (*args)[0];
	TreeNode* node;
	if (uid_s.compare("root") == 0)
	{
		node = bt->GetRootNode();
	}
	else
	{
		unsigned int uid;
		try { uid = stoul(uid_s); }
		catch (const std::invalid_argument& ia)
		{
			MSG_WARNING("Invalid argument: %s", ia.what());
			return;
		}

		

		node = bt->FindNodeById(uid);
		if (node == nullptr)
		{
			MSG_ERROR("Node %s doesn't exist", uid_s.data());
			return;
		}
	}
	

	bt->SetCurrentNode(node);
}

void BTNew(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 1, 1, "BTNew", 'n');
	if (ret == false)
		return;

	//Check if name already exists
	string bt_name = (*args)[0];

	ret = App->editor->bt_manager->IsNameAvailable(bt_name);

	if (ret == false)
	{
		MSG_ERROR("BehaviorTree name %s is already in use.", bt_name.data());
		return;
	}

	//Create a new BT
	ret = App->editor->bt_manager->CreateBT(bt_name);

	if (ret)
		MSG_INFO("BehaviorTree %s was created successfully!", bt_name.data());
	else
		MSG_ERROR("BehaviorTree %s was not created. Check for errors.", bt_name.data());
}

void ShowBTs(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 0, 0, "ShowBTs", 's');
	if (ret == false)
		return;

	App->editor->bt_manager->PrintAvailableBT();
}

void BTEdit(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 1, 1, "BTEdit", 'e');
	if (ret == false)
		return;

	string bt_name = (*args)[0];

	if (bt_name.compare("quit") == 0) //quit editing mode
	{
		App->editor->bt_manager->QuitEditingMode();
		return;
	}

	//Enable editing mode with the bt_name BT
	ret = App->editor->bt_manager->SetCurrentBT(bt_name, true);

	if (!ret)
		MSG_ERROR("BehaviorTree %s error, unable to load to edit", bt_name.data());
}

void BTInsert(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 2, 2, "BTInsert", 'i');
	if (ret == false)
		return;

	string type = (*args)[0];
	string sub_type = (*args)[1];

	BehaviorTree* bt = App->editor->bt_manager->GetCurrentBT();

	if (bt == nullptr)
	{
		MSG_WARNING("No BehaviorTree was loaded. Use: bt -e <bt_name> to start editing a BT and its BlackBoard");
		return;
	}

	ret = bt->InsertNode(type, sub_type);

	if (ret == false)
	{
		MSG_ERROR("Node %s %s could not be inserted", type.data(), sub_type.data());
	}
}

void BTList(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 0, 0, "BTList", 'l');
	if (ret == false)
		return;

	BehaviorTree* bt = App->editor->bt_manager->GetCurrentBT();
	if (bt == nullptr)
	{
		MSG_ERROR("No BT selected");
		return;
	}

	bt->PrintChildNodes();
}

void BTRemove(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 0, 1, "BTRemove", 'r');
	if (ret == false)
		return;

	BehaviorTree* bt = App->editor->bt_manager->GetCurrentBT();
	if (bt == nullptr)
	{
		MSG_ERROR("No BT selected");
		return;
	}

	if (args->size() > 0) //uid specified
	{
		string uid_s = (*args)[0];
		unsigned int uid;
		try { uid = stoul(uid_s); }
		catch (const std::invalid_argument& ia)
		{
			MSG_WARNING("Invalid argument: %s", ia.what());
			return;
		}
		bt->RemoveNode(uid);
	}
	else //remove current node
	{
		TreeNode* node = bt->GetCurrentNode();
		if (node)
		{
			bt->RemoveNode(node);
		}
		else
		{
			MSG_ERROR("There isn't a current node selected");
		}
	}

}

void RunBT(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 1, 1, "RunBT", '-');
	if (ret == false)
		return;

	string bt_name = (*args)[0];

	ret = App->editor->bt_manager->IsNameAvailable(bt_name);
	if (ret == true)
	{
		MSG_ERROR("No BehaviorTree named %s was found", bt_name.data());
		return;
	}

	ret = App->editor->bt_manager->StartRunning(bt_name);
	if (ret)
		App->editor->ChangeState(BT_RUNNING);

}

void Pause(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 0, 0, "Pause", '-');
	if (ret == false)
		return;

	App->editor->ChangeState(SLEEP);
}

void HSCardInfo(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 1, 1, "HSCardInfo", '-');
	if (ret == false)
		return;

	App->cards_db->PrintCardInfo((*args)[0]);
}

void HSLogTrack(const vector<string>* args)
{
	bool ret = CheckNumParameters(args, 0, 0, "HSLogTrack", '-');
	if (ret == false)
		return;

	App->editor->ChangeState(LOG_TRACKING);
}
