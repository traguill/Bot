#include "Player.h"
#include "ConsoleMsgs.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::MainReady()
{
	//+1 num_turns_in_play to current cards. +1 card draw +1 resources (max10)
	holding_turn = true;
	num_turn += 1;
	max_mana += 1;
}

void Player::MainStartTriggers()
{
}

void Player::MainStart()
{
	++num_cards_hand;
	--num_cards_deck;

	std::string card_name;
	if (!cards_drawn.empty())
	{
		card_name = cards_drawn.top()->name;
		cards_drawn.pop();
	}
	else
	{
		card_name = "?";
	}

	if (cards_drawn.empty() == false)
		MSG_ERROR("Player %s has more than 1 card in the drawn stack", nickname.data());

	HS_LOG("%s has drawn %s", nickname.data(), card_name.data());
}

void Player::MainNext()
{
	holding_turn = !holding_turn;
	if (holding_turn)
		++num_turn;
}

void Player::AddDrawnCard(CardObj * card)
{
	if (card)
		cards_drawn.push(card);
}
