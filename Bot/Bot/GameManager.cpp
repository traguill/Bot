#include "GameManager.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "CardsDB.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{}

void GameManager::StartGame()
{
	if (state == NO_GAMESTATE)
		NextState();
	else
		MSG_ERROR("Unable to create game. Current Game Manager state: %i", state);
}

void GameManager::SetPlayer(int num, PlayerType type)
{
	if (num == 1)
		player1 = type;
	else if (num == 2)
		player2 = type;
}

void GameManager::SetHero(int num, const std::string& hero)
{
	if (num == 1)
		hero1 = hero;
	else if (num == 2)
		hero2 = hero;

	if (player1 != PlayerType::NO_PLAYERTYPE && player2 != PlayerType::NO_PLAYERTYPE && hero1.length() != 0 && hero2.length() != 0)
		NextState();
}

void GameManager::NextState()
{
	switch (state)
	{
	case NO_GAMESTATE:
		state = CREATING;
		HS_LOG("Creating game");
		break;
	case CREATING:
		if(player1 == PlayerType::YOU)
			HS_LOG("1. YOU: %s [%s]", App->cards_db->GetName(hero1).data(), App->cards_db->GetPlayerClass(hero1).data());
		else
			HS_LOG("1. OPPONENT: %s [%s]", App->cards_db->GetName(hero1).data(), App->cards_db->GetPlayerClass(hero1).data());
		if (player2 == PlayerType::YOU)
			HS_LOG("2. YOU: %s [%s]", App->cards_db->GetName(hero2).data(), App->cards_db->GetPlayerClass(hero2).data());
		else
			HS_LOG("2. OPPONENT: %s [%s]", App->cards_db->GetName(hero2).data(), App->cards_db->GetPlayerClass(hero2).data());
		state = MULLIGAN;
		break;
	case MULLIGAN:
		break;
	default:
		break;
	}
}
