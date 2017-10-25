#include "BTManager.h"

#include "BehaviorTree.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "ModuleFileSystem.h"

BTManager::BTManager()
{
}

BTManager::~BTManager()
{
}

void BTManager::Init()
{
	vector<string> all_files;
	App->file_system->GetEnumerateFiles(".", all_files); //TODO: Make a module to handle all files and send the results to the other modules
	const char* bt_ext = "bt";
	for (vector<string>::iterator file = all_files.begin(); file != all_files.end(); ++file)
	{
		size_t point_pos = (*file).find_last_of('.');
		string extension = (*file).substr(point_pos + 1, (*file).length() - 1);
		if (extension.compare(bt_ext) == 0)
		{
			bt_files.push_back((*file).substr(0, point_pos));
		}
	}
}

void BTManager::PrintAvailableBT() const
{
	MSG_INFO(" BehaviorTrees");
	MSG_INFO("-----------------------------------------------");
	for (vector<string>::const_iterator it = bt_files.begin(); it != bt_files.end(); ++it)
	{
		MSG_INFO("* %s", (*it).data());
	}
}

bool BTManager::IsNameAvailable(const string & name) const
{
	vector<string>::const_iterator result = std::find(bt_files.begin(), bt_files.end(), name);
	return (result == bt_files.end()) ? true : false;
}

bool BTManager::CreateBT(const string & name)
{
	//IsNameAvailable should be called before this

	BehaviorTree* bt = new BehaviorTree();

	pair<map<string,BehaviorTree*>::iterator, bool> in_ret = bt_loaded.insert(pair<string, BehaviorTree*>(name, bt));

	if (in_ret.second == false)
	{
		if (bt) delete bt;
		MSG_ERROR("Error creating the BehaviorTree %s. Insertion failed.", name.data());
		return false;
	}

	bt->Init(NULL, name.data());

	bt_files.push_back(name);

	return true;
}