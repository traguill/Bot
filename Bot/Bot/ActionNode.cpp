#include "ActionNode.h"
#include "ConsoleMsgs.h"
#include "BlackBoard.h"

ActionNode::ActionNode(unsigned int uid, BlackBoard* bb) : TreeNode(uid), bb(bb)
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

bool ActionNode::AskParameters()
{
	return false;
}

void ActionNode::Load(const Data & data)
{
}

void ActionNode::Save(Data & data) const
{
}
