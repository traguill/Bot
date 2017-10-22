#ifndef __DECORATORSP_NODE_H__
#define __DECORATORSP_NODE_H__

#include "TreeNode.h"

class DecoratorSPNode : public TreeNode
{
public:
	DecoratorSPNode();
	~DecoratorSPNode();

	bool AddChild(TreeNode* child);
	bool RemoveChild(TreeNode* child);

private:

};

#endif // !__DECORATORSP_NODE_H__

