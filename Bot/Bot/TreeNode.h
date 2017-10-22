#ifndef __TREENODE_H__
#define __TREENODE_H__

#include <vector>

using namespace std;

class BehaviorTree;

enum NODETYPE
{
	ACTION,
	CONDITION,
	DECORATOR,
	DECORATOR_SP
};

enum NODERETURN
{
	SUCCESS,
	FAILURE,
	RUNNING
};

class TreeNode
{
public:
	TreeNode();
	virtual ~TreeNode();

	virtual void OnStart();
	virtual NODERETURN Run();
	virtual void OnExit();
	
	virtual bool AddChild(TreeNode* child);
	virtual bool RemoveChild(TreeNode* child);

	bool SetParent(TreeNode* parent);

protected:
	NODETYPE type;

	vector<TreeNode*> childs;
	TreeNode* parent = nullptr;

	BehaviorTree* bt; //Necessary?

};

#endif // !__TREENODE_H__
