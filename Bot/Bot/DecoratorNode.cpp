#include "DecoratorNode.h"

DecoratorNode::DecoratorNode()
{
	TreeNode::type = DECORATOR;
}

DecoratorNode::~DecoratorNode()
{
	for (auto child : childs)
	{
		delete child;
	}
}

bool DecoratorNode::AddChild(TreeNode * child)
{
	if (child != nullptr)
	{
		bool ret = child->SetParent(this);
		if(ret)
			childs.push_back(child);
		return ret;
	}
	return false;
}

bool DecoratorNode::RemoveChild(TreeNode * child)
{
	if (child != nullptr)
	{
		vector<TreeNode*>::iterator it = std::find(childs.begin(), childs.end(), child);
		if (it != childs.end())
		{
			delete *it;
			childs.erase(it);
			child = nullptr;
			return true;
		}
	}
	return false;
}
