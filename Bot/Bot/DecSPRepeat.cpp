#include "DecSPRepeat.h"
#include "ConsoleMsgs.h"

DecSPRepeat::DecSPRepeat(unsigned int uid) : DecoratorSPNode(uid)
{
	TreeNode::subtype = DECSPREPEAT;
}

DecSPRepeat::~DecSPRepeat()
{
}

void DecSPRepeat::OnStart()
{
	childs[0]->OnStart();
}

NODERETURN DecSPRepeat::Run()
{
	NODERETURN ret = childs[0]->Run(); //TODO: now never ends. specify different options.
	if (ret == SUCCESS)
	{
		childs[0]->OnExit();
		childs[0]->OnStart();
	}

	return RUNNING;
}

void DecSPRepeat::OnExit()
{
}

void DecSPRepeat::Print() const
{
	MSG_INFO("* [decoratorSP] [repeat]: %i", uid);
}

string DecSPRepeat::GetNodeHeader() const
{
	string ret = "/[DS]Repeat(" + std::to_string(uid);
	ret.append(")");
	return ret;
}
