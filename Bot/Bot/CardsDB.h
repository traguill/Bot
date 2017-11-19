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
};

class CardsDB
{
public:
	CardsDB();
	~CardsDB();

	void Init();
	void CleanUp();

	void PrintCardInfo(const std::string& c_id);

private:
	std::map<std::string, CardObj*> cards_list;
};
#endif // !__CARDSDB_H__

