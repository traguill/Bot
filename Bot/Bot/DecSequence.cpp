#include "DecSequence.h"

DecSequence::DecSequence(unsigned int uid) : DecoratorNode(uid)
{
	TreeNode::subtype = DECSEQUENCE;
}

DecSequence::~DecSequence()
{
}

void DecSequence::OnStart()
{
	current_child = 0;
}

NODERETURN DecSequence::Run()
{
	NODERETURN ret;
	bool finish = false;
	do
	{
		ret = childs[current_child]->Run();
		if (ret == SUCCESS)
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
		
	} while (ret == SUCCESS);

	return (finish) ? SUCCESS : ret;
}

void DecSequence::OnExit()
{}
