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
	bool Run();

	bool InsertNode(const string& type, const string& sub_type); //From console
	TreeNode* InsertNode(NODETYPE type, NODESUBTYPE subtype, unsigned int uid, TreeNode* parent); //From file
	void RemoveNode(unsigned int uid);
	void RemoveNode(TreeNode* node);
	
	void Save()const;

	//Helper
	void PrintChildNodes()const;

	bool SetCurrentNode(TreeNode* node);
	TreeNode* FindNodeById(unsigned int uid)const;
	TreeNode* GetCurrentNode()const;
	TreeNode* GetRootNode()const;

private:

	bool Load();
	
	void SaveNode(Data& data, TreeNode* node)const;
	TreeNode* LoadNode(Data& data, TreeNode* parent);

	unsigned int GetNewNodeUid();

	//Inserts
	bool InsertDecorator(const string& sub_type);
	TreeNode* InsertDecorator(NODESUBTYPE subtype, unsigned int uid, TreeNode* parent);
	bool InsertAction(const string& sub_type);
	TreeNode* InsertAction(NODESUBTYPE subtype, unsigned int uid, TreeNode* parent);

	//Helper Insert
	bool HandleInsertion(TreeNode* node);

	//Decorators
	bool InsertDecSequence();
	TreeNode* InsertDecSequence(unsigned int uid, TreeNode* parent);
	bool InsertDecSelector();
	TreeNode* InsertDecSelector(unsigned int uid, TreeNode* parent);

	//Actions
	bool InsertAcMove();
	TreeNode* InsertAcMove(unsigned int uid, TreeNode* parent);
	bool InsertAcClick();
	TreeNode* InsertAcClick(unsigned int uid, TreeNode* parent);
	bool InsertAcDrag();
	TreeNode* InsertAcDrag(unsigned int uid, TreeNode* parent);
	bool InsertAcWrite();
	TreeNode* InsertAcWrite(unsigned int uid, TreeNode* parent);

public:
	//Easy access
	string bt_filename; //name of the BehaviorTree file
	BlackBoard* bb = nullptr;

	string header_current_node = ""; //To show the path to the current node

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
	//Action types
	const char* ac_move = "move";
	const char* ac_click = "click";
	const char* ac_drag = "drag";
	const char* ac_write = "write";
};

#endif // !__BEHAVIORTREE_H__
