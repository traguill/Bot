#include "ConditionNode.h"

ConditionNode::ConditionNode(unsigned int uid) : TreeNode(uid)
{
	TreeNode::type = CONDITION;
}

ConditionNode::~ConditionNode()
{}