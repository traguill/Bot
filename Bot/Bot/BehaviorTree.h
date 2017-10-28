#ifndef __BEHAVIORTREE_H__
#define __BEHAVIORTREE_H__

#include <string>
#include "Data.h"
#include "TreeNode.h"

class BlackBoard;

using namespace std;

class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

	void Init(const char* filename, const char* name = nullptr);

	bool InsertNode(const string& type, const string& sub_type); //From console
	bool InsertNode(NODETYPE type, NODESUBTYPE subtype, unsigned int uid); //From file
	
	void Save()const;
private:

	bool Load();
	
	void SaveNode(Data& data, TreeNode* node)const;
	bool LoadNode(Data& data);

	unsigned int GetNewNodeUid();

	//Inserts
	bool InsertDecorator(const string& sub_type);
	bool InsertDecorator(NODESUBTYPE subtype, unsigned int uid);

	//Decorators
	bool InsertDecSequence(int id = -1);
	bool InsertDecSelector(int id = -1);

private:
	TreeNode* root = nullptr;
	TreeNode* current_node = nullptr;
	string bb_filename; //name of the BlackBoard file

	unsigned int last_uid = 0; //To asign a readable uid to a new node

	//Node types
	const char* type_decorator = "decorator";
	const char* type_action = "action";
	const char* type_condition = "condition";
	const char* type_decorator_sp = "decorator_sp";

	//Node subtypes
	//***********************************************
	//Decorator types
	const char* dec_sequence = "sequence";
	const char* dec_selector = "selector";

public:
	//Easy access
	string bt_filename; //name of the BehaviorTree file
	BlackBoard* bb = nullptr;

	string header_current_node; //To show the path to the current node
};

#endif // !__BEHAVIORTREE_H__
