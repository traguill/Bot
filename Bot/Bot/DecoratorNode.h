#ifndef __DECORATOR_NODE_H__
#define __DECORATOR_NODE_H__

#include "TreeNode.h"

class DecoratorNode : public TreeNode
{
public:
	DecoratorNode(unsigned int uid);
	~DecoratorNode();

	bool AddChild(TreeNode* child);
	bool RemoveChild(TreeNode* child); //Only removes the child from the list. Doesn't clean the memory

private:

};
#endif // !__DECORATOR_NODE_H__

