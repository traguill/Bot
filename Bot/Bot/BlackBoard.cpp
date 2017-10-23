#include "BlackBoard.h"
#include "Application.h"
#include "ModuleFileSystem.h"

BlackBoard::BlackBoard()
{
}

BlackBoard::~BlackBoard()
{
	//TODO delete vars
}

bool BlackBoard::Load(const char * filename)
{
	bool ret = false;
	char* buf = nullptr;
	unsigned int size = App->file_system->Load(filename, &buf);

	if (size == 0)
	{
		if (buf)
			delete[] buf;
		return ret;
	}

	Data bb(buf);

	size_t bb_size = bb.GetArraySize("vars");

	for (int i = 0; i < bb_size; ++i)
	{
		Data bb_var = bb.GetArray("vars", i);
		BBType bb_var_type = (BBType)bb_var.GetInt("type");
		switch (bb_var_type)
		{
		case BB_INT:
			LoadBBInt(bb_var);
			break;
		case BB_FLOAT:
			LoadBBFloat(bb_var);
			break;
		case BB_BOOL:
			LoadBBBool(bb_var);
			break;
		case BB_STRING:
			LoadBBString(bb_var);
			break;
		case BB_VECTOR:
			LoadBBVector(bb_var);
			break;
		case BB_AREA:
			LoadBBArea(bb_var);
			break;
		}
	}

	if (buf)
		delete[] buf;

	return true;
}

void BlackBoard::LoadBBInt(Data & data)
{
	BBVar* var = new BBVar();
	var->type = BB_INT;
	var->name = data.GetString("name");
	var->value = new char[sizeof(int)];
	int data_val = data.GetInt("value");
	memcpy_s(var->value, sizeof(int), &data_val, sizeof(int));

	bb_vars.insert(pair<string, BBVar*>(var->name, var));
}

void BlackBoard::LoadBBFloat(Data & data)
{
	BBVar* var = new BBVar();
	var->type = BB_FLOAT;
	var->name = data.GetString("name");
	var->value = new char[sizeof(float)];
	float data_val = data.GetFloat("value");
	memcpy_s(var->value, sizeof(float), &data_val, sizeof(float));

	bb_vars.insert(pair<string, BBVar*>(var->name, var));
}

void BlackBoard::LoadBBString(Data & data)
{
	BBVar* var = new BBVar();
	var->type = BB_STRING;
	var->name = data.GetString("name");
	const char* data_val = data.GetString("value");
	size_t data_size = sizeof(char)*(strlen(data_val)+1);
	var->value = new char[sizeof(data_val)];
	memcpy_s(var->value, data_size, &data_val, data_size);

	bb_vars.insert(pair<string, BBVar*>(var->name, var));
}

void BlackBoard::LoadBBBool(Data & data)
{
	BBVar* var = new BBVar();
	var->type = BB_BOOL;
	var->name = data.GetString("name");
	var->value = new char[sizeof(bool)];
	bool data_val = data.GetBool("value");
	memcpy_s(var->value, sizeof(bool), &data_val, sizeof(bool));

	bb_vars.insert(pair<string, BBVar*>(var->name, var));
}

void BlackBoard::LoadBBVector(Data & data)
{
	//value = type(int) + size(int)
}

void BlackBoard::LoadBBArea(Data & data)
{
	//value = area_name(string) ->Search the area in AreaManager pick the pointer.
}
