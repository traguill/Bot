#include "AcKey.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include <iostream>

AcKey::AcKey(unsigned int uid, BlackBoard * bb) : ActionNode(uid, bb)
{
	TreeNode::subtype = ACKEY;
}

AcKey::~AcKey()
{
}

void AcKey::OnStart()
{
	App->keyboard->InitKey(key);
}

NODERETURN AcKey::Run()
{
	bool ret = App->keyboard->PressKey();
	if (ret)
		return SUCCESS;
	else
		return RUNNING;
}

void AcKey::OnExit()
{
}

void AcKey::Load(const Data & data)
{
	key_string = data.GetString("key_string");
	key = (SPECIAL_KEY)data.GetUInt("key");
}

void AcKey::Save(Data & data) const
{
	data.AppendString("key_string", key_string.data());
	data.AppendUInt("key", key);
}

void AcKey::Print() const
{
	MSG_INFO("* [action] [key]: %i ~key:%s", uid, key_string.data());
}

string AcKey::GetNodeHeader() const
{
	string ret = "/[A]Key(" + std::to_string(uid);
	ret.append(")");
	return ret;
}

bool AcKey::AskParameters()
{
	bool finish = false;

	while (finish == false)
	{
		MSG_INFO("Special key:");
		getline(cin, key_string);

		if (key_string.compare("enter") == 0)
		{
			key = ENTER;
			finish = true;
		}
		else
		{
			MSG_ERROR("Invalid special key");
		}
	}
	
	return true;
}
