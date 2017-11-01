#include "AcWrite.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "KeyboardEmulator.h"

#include <iostream>

AcWrite::AcWrite(unsigned int uid, BlackBoard * bb) : ActionNode(uid, bb)
{
	TreeNode::subtype = ACWRITE;
}

AcWrite::~AcWrite()
{
}

void AcWrite::OnStart()
{
	App->keyboard->InitWrite(text);
}

NODERETURN AcWrite::Run()
{
	bool ret = App->keyboard->Write();
	if (ret)
		return SUCCESS;
	else
		return RUNNING;
}

void AcWrite::OnExit()
{}

void AcWrite::Load(const Data & data)
{
	text = data.GetString("text");
}

void AcWrite::Save(Data & data) const
{
	data.AppendString("text", text.data());
}

void AcWrite::Print() const
{
	MSG_INFO("* [action] [write]: %i ~text:%s", uid, text.data());
}

string AcWrite::GetNodeHeader() const
{
	string ret = "/[A]Text(" + std::to_string(uid);
	ret.append(")");
	return ret;
}

bool AcWrite::AskParameters()
{
	MSG_INFO("Text:");
	getline(cin, text);

	return true;
}
