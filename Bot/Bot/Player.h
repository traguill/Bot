#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "CardsDB.h"
#include <string>
#include <stack>

class Player
{
public:
	Player();
	~Player();

	void MainReady();
	void MainStartTriggers();
	void MainStart();
	void MainNext();

	void AddDrawnCard(CardObj* card);

public:
	int id = 0; //Player 1 or 2
	CardObj* hero = nullptr;
	std::string nickname;

	bool holding_turn = false; //Is this player's turn?
	int num_turn = 0; //How many turns has played
	int num_cards_hand = 0;
	int num_cards_deck = 30;
	int mana_available = 0; //Mana available for this turn
	int max_mana = 0; //Maximum mana available
	int mana_spent = 0; //Mana spent this turn
	int total_mana_spend = 0; //Mana spent during the whole game

	std::stack<CardObj*> cards_drawn;
	
};
#endif // !__PLAYER_H__
