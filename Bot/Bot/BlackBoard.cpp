#include "BlackBoard.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ConsoleMsgs.h"
#include "Editor.h"
#include "AreaManager.h"

BlackBoard::BlackBoard()
{
}

BlackBoard::~BlackBoard()
{
	Save();
	for (map<string, BBVar*>::iterator it = bb_vars.begin(); it != bb_vars.end(); ++it)
	{
		if (it->second != nullptr)
		{
			if(it->second->value && it->second->type != BB_STRING && it->second->type != BB_AREA)
				delete[] ((*it).second->value);
			delete it->second;
		}
	}
}

void BlackBoard::Init(const char* filename, const char* name)
{
	if (filename == NULL)
	{
		if (name == nullptr)
		{
			MSG_ERROR("A BlackBoard could not be initialize. No filename nor name");
			return;
		}
		this->filename = name;
		Save();
	}
	else
	{
		this->filename = filename;
		bool ret = Load();
		if (ret)
		{
			MSG_INFO("BlackBoard loaded successfully.");
		}
		else
		{
			MSG_ERROR("An error occurred while loading BlackBoard");
		}
	}
	
}

bool BlackBoard::Save() const
{
	Data data;

	data.AppendArray("vars");
	
	for (map<string, BBVar*>::const_iterator it = bb_vars.begin(); it != bb_vars.end(); ++it)
	{
		switch ((*it).second->type)
		{
		case BB_INT:
			SaveBBInt(data, (*it).second);
			break;
		case BB_FLOAT:
			SaveBBFloat(data, (*it).second);
			break;
		case BB_BOOL:
			SaveBBBool(data, (*it).second);
			break;
		case BB_STRING:
			SaveBBString(data, (*it).second);
			break;
		case BB_VECTOR:
			SaveBBVector(data, (*it).second);
			break;
		case BB_AREA:
			SaveBBArea(data, (*it).second);
			break;
		}
	}

	char* buf = nullptr;
	size_t size = data.Serialize(&buf);

	if (size <= 0)
	{
		if (buf)
			delete[] buf;
		return false;
	}

	size_t ret_size = App->file_system->Save(filename.data(), buf, size);
	
	return (ret_size > 0) ? true : false;
}

bool BlackBoard::Load()
{
	bool ret = false;
	char* buf = nullptr;
	unsigned int size = App->file_system->Load(filename.data(), &buf);

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

void BlackBoard::PrintVars() const
{
	MSG_INFO("BlackBoard vars: ");
	MSG_INFO("----------------------------------------------");
	for (map<string, BBVar*>::const_iterator it = bb_vars.begin(); it != bb_vars.end(); ++it)
	{
		switch ((*it).second->type)
		{
		case BB_INT:
			MSG_INFO("* %s: %i", it->first.data(), *(reinterpret_cast<int*>((*it).second->value)));
			break;
		case BB_FLOAT:
			MSG_INFO("* %s: %f", it->first.data(), *(reinterpret_cast<float*>((*it).second->value)));
			break;
		case BB_BOOL:
			MSG_INFO("* %s: %i", it->first.data(), *(reinterpret_cast<bool*>((*it).second->value)));
			break;
		case BB_STRING:
			MSG_INFO("* %s: %s", it->first.data(), it->second->value);
			break;
		case BB_VECTOR:
			break;
		case BB_AREA:
			MSG_INFO("* Area %s: %s", it->first.data(), ((Area*)(it->second->value))->name.data());
			break;
		default:
			break;
		}
	}
}

void * BlackBoard::FindVar(const string & name, BBType type) const
{
	map<string, BBVar*>::const_iterator result = bb_vars.find(name);
	if (result == bb_vars.end())
		return nullptr;
	
	if (result->second->type == type) //Match the type
		return result->second->value;

	return nullptr;
}

bool BlackBoard::InsertBool(const string & name, bool value)
{
	BBVar* var = new BBVar();
	var->name = name.data();
	var->type = BB_BOOL;
	var->value = new char[sizeof(bool)];
	memcpy(var->value, &value, sizeof(bool));

	pair<map<string, BBVar*>::iterator, bool> ret = bb_vars.insert(pair<string, BBVar*>(name, var));

	if (ret.second == false)
	{
		delete[] var->value;
		delete var;
	}

	return ret.second;
}

bool BlackBoard::InsertInt(const string & name, int value)
{
	BBVar* var = new BBVar();
	var->name = name.data();
	var->type = BB_INT;
	var->value = new char[sizeof(int)];
	memcpy(var->value, &value, sizeof(int));

	pair<map<string, BBVar*>::iterator, bool> ret = bb_vars.insert(pair<string, BBVar*>(name, var));

	if (ret.second == false)
	{
		delete[] var->value;
		delete var;
	}

	return ret.second;
}

bool BlackBoard::InsertFloat(const string & name, float value)
{
	BBVar* var = new BBVar();
	var->name = name.data();
	var->type = BB_FLOAT;
	var->value = new char[sizeof(float)];
	memcpy(var->value, &value, sizeof(float));

	pair<map<string, BBVar*>::iterator, bool> ret = bb_vars.insert(pair<string, BBVar*>(name, var));

	if (ret.second == false)
	{
		delete[] var->value;
		delete var;
	}

	return ret.second;
}

bool BlackBoard::InsertString(const string & name, const string & value)
{
	BBVar* var = new BBVar();
	var->name = name.data();
	var->type = BB_STRING;
	size_t size = sizeof(char)*(value.length() + 1);
	var->value = new char[size];
	memcpy(var->value, value.data(), size);

	pair<map<string, BBVar*>::iterator, bool> ret = bb_vars.insert(pair<string, BBVar*>(name, var));

	if (ret.second == false)
	{
		delete[] var->value;
		delete var;
	}

	return ret.second;
}

bool BlackBoard::InsertArea(const string & name, const string & value)
{
	//Same as string but first search if area exists
	bool exists = App->editor->area_manager->ExistsArea(value);
	if (exists == false)
		return false;

	BBVar* var = new BBVar();
	var->name = name.data();
	var->type = BB_AREA;
	Area* a = App->editor->area_manager->FindArea(value);
	var->value = (char*)a;

	pair<map<string, BBVar*>::iterator, bool> ret = bb_vars.insert(pair<string, BBVar*>(name, var));

	if (ret.second == false)
	{
		delete[] var->value;
		delete var;
	}

	return ret.second;
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
	memcpy_s(var->value, data_size, data_val, data_size);

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
	BBVar* var = new BBVar();
	var->type = BB_AREA;
	var->name = data.GetString("name");
	const char* data_val = data.GetString("value");

	Area* a = App->editor->area_manager->FindArea(string(data_val));

	if (a == nullptr)
	{
		MSG_ERROR("Area %s not loaded. This area does not exist.", var->name.data());
		delete var;
		return;
	}

	var->value = (char*)a; //Too hacky?

	bb_vars.insert(pair<string, BBVar*>(var->name, var));
}

void BlackBoard::SaveBBInt(Data & data, const BBVar* var) const
{
	Data data_var;
	data_var.AppendString("name", var->name.data());
	data_var.AppendInt("type", var->type);
	data_var.AppendInt("value", *(reinterpret_cast<int*>(var->value)));

	data.AppendArrayValue(data_var);
}

void BlackBoard::SaveBBFloat(Data & data, const BBVar* var) const
{
	Data data_var;
	data_var.AppendString("name", var->name.data());
	data_var.AppendInt("type", var->type);
	data_var.AppendFloat("value", *(reinterpret_cast<float*>(var->value)));

	data.AppendArrayValue(data_var);
}

void BlackBoard::SaveBBString(Data & data, const BBVar* var) const
{
	Data data_var;
	data_var.AppendString("name", var->name.data());
	data_var.AppendInt("type", var->type);
	data_var.AppendString("value", var->value);

	data.AppendArrayValue(data_var);
}

void BlackBoard::SaveBBBool(Data & data, const BBVar* var) const
{
	Data data_var;
	data_var.AppendString("name", var->name.data());
	data_var.AppendInt("type", var->type);
	data_var.AppendBool("value", *(reinterpret_cast<bool*>(var->value)));

	data.AppendArrayValue(data_var);
}

void BlackBoard::SaveBBVector(Data & data, const BBVar* var) const
{
}

void BlackBoard::SaveBBArea(Data & data, const BBVar* var) const
{
	Data data_var;
	data_var.AppendString("name", var->name.data());
	data_var.AppendInt("type", var->type);

	string name = ((Area*)(var->value))->name; //This must be wrong for some reason. But it works...

	data_var.AppendString("value", name.data());

	data.AppendArrayValue(data_var);
}
