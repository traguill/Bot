#include "CardsDB.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Data.h"
#include "ConsoleMsgs.h"

CardsDB::CardsDB()
{
}

CardsDB::~CardsDB()
{
}

void CardsDB::Init()
{
	char* buf = nullptr;
	unsigned int size = App->file_system->Load("res/cards.json", &buf);
	if (size > 0)
	{
		Data inf(buf);

		int cards_size = inf.GetArraySize("cards");
		
		for (int i = 0; i < cards_size; ++i)
		{
			Data card = inf.GetArray("cards", i);

			CardObj* c_obj = new CardObj();
			c_obj->id = card.GetString("id");
			c_obj->name = card.GetString("name");
			c_obj->attack = card.GetInt("attack");
			c_obj->cost = card.GetInt("cost");
			c_obj->health = card.GetInt("health");

			cards_list.insert(pair<std::string, CardObj*>(c_obj->id, c_obj));
		}
	}

	if (buf)
		delete[] buf;
}

void CardsDB::CleanUp()
{
	for (auto card : cards_list)
	{
		if(card.second)
			delete card.second;
	}

	cards_list.clear();
}

void CardsDB::PrintCardInfo(const std::string & c_id)
{
	std::map<std::string, CardObj*>::iterator it = cards_list.find(c_id);
	if (it != cards_list.end())
	{
		MSG_INFO("%s", it->second->name.data());
		MSG_INFO("---------------------------------");
		MSG_INFO("Cost: %i", it->second->cost);
		MSG_INFO("Attack: %i", it->second->attack);
		MSG_INFO("Health: %i", it->second->health);
	}
	else
	{
		MSG_WARNING("Card %s not found", c_id.data());
	}
}