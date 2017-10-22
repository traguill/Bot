#include "ActionNode.h"
#include "ConsoleMsgs.h"

ActionNode::ActionNode()
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