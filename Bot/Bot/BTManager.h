#ifndef __BTMANAGER_H__
#define __BTMANAGER_H__

#include <vector>
#include <map>
#include <string>

using namespace std;

class BehaviorTree;

class BTManager
{
public:
	BTManager();
	~BTManager();
	
	void Init();

	void PrintAvailableBT()const;

	bool IsNameAvailable(const string& name)const; //Checks if 'name' is in use in another BT.
	bool CreateBT(const string& name);

private:

	vector<string> bt_files; //BT available to load.
	map<string, BehaviorTree*> bt_loaded; 

};
#endif // !__BTMANAGER_H__

