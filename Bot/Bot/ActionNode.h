#ifndef __ACTION_NODE_H__
#define __ACTION_NODE_H__

#include "TreeNode.h"
#include "Data.h"

class BlackBoard;

class ActionNode : public TreeNode
{
public:
	ActionNode(unsigned int uid, BlackBoard* bb);
	~ActionNode();

	virtual bool AskParameters();

	virtual void Load(const Data& data);
	virtual void Save(Data& data)const;
protected:
	BlackBoard* bb;
};
#endif // !__ACTION_NODE_H__
