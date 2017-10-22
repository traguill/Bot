#include "TreeNode.h"
#include "BehaviorTree.h"
#include "ConsoleMsgs.h"

TreeNode::TreeNode()
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

bool TreeNode::SetParent(TreeNode * parent)
{
	if (parent != nullptr)
	{
		this->parent = parent;
		return true;
	}

	return false;
}
