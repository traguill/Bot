#include "AcDrag.h"
#include "AreaManager.h"
#include "Application.h"
#include "MouseEmulator.h"
#include "Editor.h"
#include "BlackBoard.h"
#include <iostream>

#include "ConsoleMsgs.h"

AcDrag::AcDrag(unsigned int uid, BlackBoard * bb) : ActionNode(uid, bb)
{
	TreeNode::subtype = ACDRAG;
}

AcDrag::~AcDrag()
{
}

void AcDrag::OnStart()
{
	App->mouse->Clear();
	Point<int> p_dst;
	App->editor->area_manager->GetRndPointArea(area_dst, p_dst);
	App->mouse->AddPoint(p_dst.x, p_dst.y);
	App->mouse->InitMovement(delay);
	App->mouse->LeftClick();
}

NODERETURN AcDrag::Run()
{
	bool ret = App->mouse->Move(App->dt);
	if (ret == false)
		return RUNNING;
	else
		return SUCCESS;
}

void AcDrag::OnExit()
{
	App->mouse->Clear();
	App->mouse->LeftClickUp();
}

void AcDrag::Load(const Data & data)
{
	delay = data.GetFloat("delay");
	area_name = data.GetString("bb_area");
	void* ret = bb->FindVar(area_name, BBType::BB_AREA);
	if (ret == nullptr)
		MSG_ERROR("Action drag %i: Invalid area name", uid);
	else
		area_dst = (Area*)ret;
}

void AcDrag::Save(Data & data) const
{
	if (area_dst)
		data.AppendString("bb_area", area_name.data());
	data.AppendFloat("delay", delay);
}

void AcDrag::Print() const
{
	MSG_INFO("* [action] [drag]: %i ~area:%s, delay:%.3f", uid, area_name.data(), delay);
}

string AcDrag::GetNodeHeader() const
{
	string ret = "/[A]Drag(" + std::to_string(uid);
	ret.append(")");
	return ret;
}

bool AcDrag::AskParameters()
{
	area_dst = nullptr; //This should be null already. But to prevent future errors

	while (area_dst == nullptr) //WARNING: if no area was defined in the BB this will cause an endless loop 
	{
		MSG_INFO("Area:");
		string msg;
		getline(cin, msg);

		void* ret = bb->FindVar(msg, BBType::BB_AREA);
		if (ret == nullptr)
			MSG_WARNING("Invalid area name");
		else
		{
			area_dst = (Area*)ret;
			area_name = msg;
		}
	}

	MSG_INFO("Delay(s): ");
	string s_delay;
	getline(cin, s_delay);

	delay = stof(s_delay);

	return true;
}
