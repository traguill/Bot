#include "AcClick.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "MouseEmulator.h"

#include <iostream>

AcClick::AcClick(unsigned int uid, BlackBoard * bb) : ActionNode(uid, bb)
{
	TreeNode::subtype = ACCLICK;
}

AcClick::~AcClick()
{
}

void AcClick::OnStart()
{
	App->mouse->InitClick(delay);
}

NODERETURN AcClick::Run()
{
	bool ret = App->mouse->Clicking();
	if (ret == true)
		return SUCCESS;
	else
		return RUNNING;
}

void AcClick::OnExit()
{
}

void AcClick::Load(const Data & data)
{
	delay = data.GetFloat("delay");
}

void AcClick::Save(Data & data) const
{
	data.AppendFloat("delay", delay);
}

void AcClick::Print() const
{
	MSG_INFO("* [action] [click]: %i ~delay:%.3f", uid, delay);
}

string AcClick::GetNodeHeader() const
{
	string ret = "/[A]Click(" + std::to_string(uid);
	ret.append(")");
	return ret;
}

bool AcClick::AskParameters()
{
	MSG_INFO("Delay:");
	string msg;
	getline(cin, msg);

	delay = stof(msg);

	return true;
}
