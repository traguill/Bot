#include "DecoratorSPNode.h"

DecoratorSPNode::DecoratorSPNode()
{
	TreeNode::type = DECORATOR_SP;
}

DecoratorSPNode::~DecoratorSPNode()
{
	for (auto child : childs)
	{
		delete child;
	}
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
		delete child;
		child = nullptr;
		childs.clear();
		return true;
	}
	return false;
}
