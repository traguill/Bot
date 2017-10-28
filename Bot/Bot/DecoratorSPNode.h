#ifndef __DECORATORSP_NODE_H__
#define __DECORATORSP_NODE_H__

#include "TreeNode.h"

class DecoratorSPNode : public TreeNode
{
public:
	DecoratorSPNode(unsigned int uid);
	~DecoratorSPNode();

	bool AddChild(TreeNode* child);
	bool RemoveChild(TreeNode* child);  //Only removes the child from the list. Doesn't clean the memory

private:

};

#endif // !__DECORATORSP_NODE_H__

