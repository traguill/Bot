#include "BehaviorTree.h"

#include "TreeNode.h"
#include "ConsoleMsgs.h"
#include "BlackBoard.h"
#include "Data.h"
#include "ModuleFileSystem.h"
#include "Application.h"

BehaviorTree::BehaviorTree()
{
}

BehaviorTree::~BehaviorTree()
{
	if (bb)
		delete bb;
}

void BehaviorTree::Init(const char * filename, const char* name)
{
	if (filename == nullptr) //Just created
	{
		if (name == nullptr)
		{
			MSG_ERROR("A BehaviorTree could not be initialize. No filename nor name");
			return;
		}

		bt_filename = name; bt_filename.append(".bt");
		bb_filename = name; bb_filename.append(".bb");//Maybe I shouldn't let the user change the name of the bb. This could bring trouble...

		bb = new BlackBoard();
		bb->Init(NULL, bb_filename.data());

		Save();
	}
	else //Load from file
	{
		bt_filename = filename; bt_filename.append(".bt");
		Load();
	}
}

bool BehaviorTree::Load()
{
	char* buf = nullptr;
	size_t size = App->file_system->Load(bt_filename.data(), &buf);
	if (size <= 0)
	{
		MSG_ERROR("Could not load BehaviorTree %s", bt_filename.data());
		if (buf)
			delete[] buf;
		return false;
	}

	Data data(buf);

	bb_filename = data.GetString("bb_name");

	if (buf)
		delete[] buf;

	bb = new BlackBoard();
	bb->Init(bb_filename.data());

	return true;
}

void BehaviorTree::Save() const
{
	Data data;

	data.AppendString("bb_name", bb_filename.data());

	char* buf = nullptr;
	size_t size = data.Serialize(&buf);

	if (size > 0)
	{
		size = App->file_system->Save(bt_filename.data(), buf, size);
		if (size <= 0)
		{
			MSG_ERROR("Error while saving the BT %s", bt_filename.data());
		}
	}
	else
	{
		MSG_ERROR("Error while serializing the BT %s", bt_filename.data());
	}
	
	if (buf)
		delete[] buf;
}
