#ifndef __CARDSDB_H__
#define __CARDSDB_H__

#include <string>
#include <map>
//TODO: Make it a class

struct CardObj
{
	std::string id;
	std::string name;
	int cost;
	int attack;
	int health;
	std::string playerClass;
	std::string type;
};

class CardsDB
{
public:
	CardsDB();
	~CardsDB();

	void Init();
	void CleanUp();

	void PrintCardInfo(const std::string& c_id);

	std::string GetName(const std::string& card_id);
	std::string GetPlayerClass(const std::string& card_id);
	std::string GetType(const std::string& card_id);

	CardObj* GetCardObj(const std::string& card_id);

private:
	std::map<std::string, CardObj*> cards_list;
};
#endif // !__CARDSDB_H__

