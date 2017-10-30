#ifndef __BTMANAGER_H__
#define __BTMANAGER_H__

#include <vector>
#include <map>
#include <string>
#include "ConsoleHeader.h"

using namespace std;

class BehaviorTree;

class BTManager : public ConsoleHeader
{
public:
	BTManager();
	~BTManager();
	
	void Init();

	void PrintAvailableBT()const;

	bool IsNameAvailable(const string& name)const; //Checks if 'name' is in use in another BT.
	bool CreateBT(const string& name);

	bool SetCurrentBT(const string& name, bool editing_mode = false);
	void QuitEditingMode();
	BehaviorTree* GetCurrentBT()const;

	bool StartRunning(const string& bt);
	bool Run();

	void PrintHeader();

private:

	vector<string> bt_files; //BT available to load.
	map<string, BehaviorTree*> bt_loaded; 
	BehaviorTree* current_bt = nullptr;

	bool editing_mode = false;
	bool running = false;

};
#endif // !__BTMANAGER_H__

