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

	}
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
