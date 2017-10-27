#include "ActionNode.h"
#include "ConsoleMsgs.h"

ActionNode::ActionNode(unsigned int uid) : TreeNode(uid)
{
	TreeNode::type = ACTION;
}

ActionNode::~ActionNode()
{
	if (childs.size() > 0)
	{
		MSG_ERROR("Action node contains children. Remove failed."); //Should not happen.
	}
}