#include "AcMove.h"
#include "AreaManager.h"

#include <iostream>

#include "ConsoleMsgs.h"
#include "MouseEmulator.h"
#include "Application.h"
#include "Editor.h"
#include "BlackBoard.h"

AcMove::AcMove(unsigned int uid, BlackBoard* bb) : ActionNode(uid, bb)
{
	TreeNode::subtype = ACMOVE;
}

AcMove::~AcMove()
{}

void AcMove::OnStart()
{
	MSG_INFO("MOVE(%i): START", uid);
	App->mouse->Clear();
	Point<int> p_dst;
	App->editor->area_manager->GetRndPointArea(area_dst, p_dst);
	MSG_WARNING("Area %s - Point x: %i y: %i", area_dst->name.data(), p_dst.x, p_dst.y);
	App->mouse->AddPoint(p_dst.x, p_dst.y);
	App->mouse->InitMovement(0);
}

NODERETURN AcMove::Run()
{
	bool ret = App->mouse->Move(App->dt);
	if (ret == false)
		return RUNNING;
	else
		return SUCCESS;
}

void AcMove::OnExit()
{
	MSG_INFO("MOVE(%i): START", uid);
	App->mouse->Clear();
}

void AcMove::Load(const Data & data)
{
	area_name = data.GetString("bb_area");
	void* ret = bb->FindVar(area_name, BBType::BB_AREA);
	if (ret == nullptr)
		MSG_ERROR("Action move %i: Invalid area name", uid);
	else
		area_dst = (Area*)ret;
}

void AcMove::Save(Data & data) const
{
	if (area_dst)
		data.AppendString("bb_area", area_name.data());
}

void AcMove::Print() const
{
	MSG_INFO("* [action] [move]: %i ~area:%s", uid, area_name.data());
}

string AcMove::GetNodeHeader() const
{
	string ret = "/[A]Move(" + std::to_string(uid);
	ret.append(")");
	return ret;
}

bool AcMove::AskParameters()
{
	area_dst = nullptr; //This should be null already. But to prevent future errors

	while (area_dst == nullptr) //WARNING: if no area was defined in the BB this will cause an endless loop 
	{
		MSG_INFO("Area: destination");
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
	
	return true;
}
