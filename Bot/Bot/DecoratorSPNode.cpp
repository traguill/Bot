#include "DecoratorSPNode.h"

DecoratorSPNode::DecoratorSPNode(unsigned int uid) : TreeNode(uid)
{
	TreeNode::type = DECORATOR_SP;
}

DecoratorSPNode::~DecoratorSPNode()
{
	//Handled outside
}

bool DecoratorSPNode::AddChild(TreeNode * child)
{
	if (child != nullptr)
	{
		if (childs.size() == 0)
		{
			childs.push_back(child);
			return true;
		}
	}
	return false;
}

bool DecoratorSPNode::RemoveChild(TreeNode * child)
{
	if (childs.front() == child) //Only has one child
	{
		childs.clear();
		return true;
	}
	return false;
}
