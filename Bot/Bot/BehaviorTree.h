#ifndef __BEHAVIORTREE_H__
#define __BEHAVIORTREE_H__

#include <string>

class TreeNode;
class BlackBoard;

using namespace std;

class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

	void Init(const char* filename, const char* name = nullptr);

private:

	bool Load();
	void Save()const;

private:
	TreeNode* root = nullptr;
	TreeNode* current_node = nullptr;
	BlackBoard* bb = nullptr;
	string bb_filename; //name of the BlackBoard file

public:
	//Easy access
	string bt_filename; //name of the BehaviorTree file
};

#endif // !__BEHAVIORTREE_H__
