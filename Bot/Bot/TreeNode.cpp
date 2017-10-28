#include "TreeNode.h"
#include "BehaviorTree.h"
#include "ConsoleMsgs.h"

TreeNode::TreeNode(unsigned int uid) : uid(uid)
{
}

TreeNode::~TreeNode()
{
}

void TreeNode::OnStart()
{
}

NODERETURN TreeNode::Run()
{
	return NODERETURN::FAILURE;
}

void TreeNode::OnExit()
{
}

bool TreeNode::AddChild(TreeNode * child)
{
	MSG_ERROR("This type of node doesn't admit children");
	return false;
}

bool TreeNode::RemoveChild(TreeNode * child)
{
	MSG_ERROR("This type of node doesn't admit children");
	return false;
}

bool TreeNode::HasChilds() const
{
	return (childs.size() > 0) ? true : false;
}

const vector<TreeNode*> TreeNode::GetChilds() const
{
	return childs;
}

bool TreeNode::SetParent(TreeNode * parent)
{
	if (parent != nullptr)
	{
		this->parent = parent;
		return true;
	}

	return false;
}

void TreeNode::GetTypeSubType(NODETYPE & type, NODESUBTYPE & subtype)
{
	type = this->type;
	subtype = this->subtype;
}

unsigned int TreeNode::GetUid() const
{
	return uid;
}

void TreeNode::Print() const
{
}
