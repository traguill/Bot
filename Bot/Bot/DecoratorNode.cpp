#include "DecoratorNode.h"

DecoratorNode::DecoratorNode(unsigned int uid) : TreeNode(uid)
{
	TreeNode::type = DECORATOR;
}

DecoratorNode::~DecoratorNode()
{
	//Handled outside
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
			childs.erase(it);
			return true;
		}
	}
	return false;
}
