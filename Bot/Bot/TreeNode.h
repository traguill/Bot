#ifndef __TREENODE_H__
#define __TREENODE_H__

#include <vector>
#include <string>

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

enum NODESUBTYPE
{
	DECSELECTOR,
	DECSEQUENCE
};

class TreeNode
{
public:
	TreeNode(unsigned int uid);
	virtual ~TreeNode();

	virtual void OnStart();
	virtual NODERETURN Run();
	virtual void OnExit();
	
	virtual bool AddChild(TreeNode* child);
	virtual bool RemoveChild(TreeNode* child);
	bool HasChilds()const;
	const vector<TreeNode*> GetChilds()const;

	TreeNode* GetParent()const;
	bool SetParent(TreeNode* parent);

	void GetTypeSubType(NODETYPE& type, NODESUBTYPE& subtype);
	unsigned int GetUid()const;

	virtual void Print()const; //Prints the node type/subtype/id in the console
	virtual string GetNodeHeader()const;

protected:
	NODETYPE type;
	NODESUBTYPE subtype;

	vector<TreeNode*> childs;
	TreeNode* parent = nullptr;

	unsigned int uid;
};

#endif // !__TREENODE_H__
