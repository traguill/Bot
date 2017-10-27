#include "DecSelector.h"

DecSelector::DecSelector(unsigned int uid) : DecoratorNode(uid)
{
	TreeNode::subtype = DECSELECTOR;
}

DecSelector::~DecSelector()
{
}

void DecSelector::OnStart()
{
	current_child = 0;
}

NODERETURN DecSelector::Run()
{
	NODERETURN ret;
	bool finish = false;
	do
	{
		ret = childs[current_child]->Run();
		if (ret == FAILURE)
		{
			childs[current_child]->OnExit();

			++current_child;
			if (current_child >= childs.size())
			{
				finish = true;
				break;
			}
			else
			{
				childs[current_child]->OnStart();
			}
		}

	} while (ret == FAILURE);

	return (finish) ? FAILURE : ret;
}

void DecSelector::OnExit()
{}
