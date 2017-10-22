#ifndef __BEHAVIORTREE_H__
#define __BEHAVIORTREE_H__

class TreeNode;

class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

private:
	TreeNode* root = nullptr;
	TreeNode* current_node = nullptr;
};

#endif // !__BEHAVIORTREE_H__
