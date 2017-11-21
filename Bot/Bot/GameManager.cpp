#include "GameManager.h"
#include "ConsoleMsgs.h"
#include "Application.h"
#include "CardsDB.h"
#include "Player.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{}

void GameManager::StartGame()
{
	Reset();

	p_ai = new Player();
	p_op = new Player();

	NextState();
}

void GameManager::SetPlayer(int num, PlayerType type)
{
	Player* tmp = nullptr;
	switch (type)
	{
	case OPPONENT:
		tmp = p_op;
		break;
	case YOU:
		tmp = p_ai;
		ai_id = num;
		break;
	}
	if (tmp)
	{
		tmp->id = num;
		if (num == 1)
		{
			current_player = tmp;
			tmp->num_cards_hand = 3;
			tmp->num_cards_deck -= 3;
		}
		else if (num == 2)
		{
			tmp->num_cards_hand = 5; //4 + coin
			tmp->num_cards_deck -= 4;
		}
	}
}

void GameManager::SetHero(int num, const std::string& hero)
{
	if (p_op->id == num)
		p_op->hero = App->cards_db->GetCardObj(hero);
	else if (p_ai->id == num)
		p_ai->hero = App->cards_db->GetCardObj(hero);

	if (p_op->hero != nullptr && p_ai->hero != nullptr)
		NextState();
}

void GameManager::SetNickname(int num, const std::string & nick)
{
	if (p_op->id == num)
		p_op->nickname = nick;
	else if (p_ai->id == num)
		p_ai->nickname = nick;
}

void GameManager::InsertMulliganCard(const std::string & card_id)
{
	mulligan_cards.push_back(card_id);
}

void GameManager::CardDrawn(const std::string & card_id)
{
	p_ai->AddDrawnCard(App->cards_db->GetCardObj(card_id));
}

void GameManager::ChangeState(GameState new_state)
{
	switch (new_state)
	{
	case BEGIN_MULLIGAN:
		HS_LOG("Mulligan cards---------------------");
		for (auto c : mulligan_cards)
		{
			std::string card_name = App->cards_db->GetName(c);
			if (card_name.length() != 0)
				HS_LOG("%s", card_name.data());
			else
				HS_LOG("Card %s not registered", c.data());
		}
		HS_LOG("-----------------------------------");
		break;
	case MAIN_READY:
		if (state == BEGIN_MULLIGAN) //Print nicknames
		{
			HS_LOG("AI(%i): %s - %s", p_ai->id, p_ai->nickname.data(), p_ai->hero->name.data());
			HS_LOG("Opponent(%i): %s - %s", p_op->id, p_op->nickname.data(), p_op->hero->name.data());
			HS_LOG("-----------------------------------");
		}
		current_player->MainReady();
		break;
	case MAIN_START_TRIGGERS:
		//apply triggers effects at the begining of the turn
		current_player->MainStartTriggers();
		break;
	case MAIN_START:
		//Draw a card
		current_player->MainStart();
		break;
	case MAIN_ACTION:
		break;
	case MAIN_END:
		break;
	case MAIN_CLEANUP:
		break;
	case MAIN_NEXT:
		HS_LOG("END TURN");
		//switch player turn
		current_player = (current_player == p_ai) ? p_op : p_ai;
		p_op->MainNext();
		p_ai->MainNext();
		break;
	}
	state = new_state;
}

Player * GameManager::StringToPlayer(const std::string & line, size_t start)
{
	size_t ai_size = p_ai->nickname.length();
	size_t op_size = p_op->nickname.length();
	size_t i = 0;
	char c;
	while (ai_size < i && op_size < i)
	{
		if (p_ai->nickname[i] != p_op->nickname[i])
		{
			c = line[start + i];
			if (c == p_ai->nickname[i])
				return p_ai;
			else
				return p_op;
		}
		else
			++i;
	}
	return nullptr;
}

void GameManager::ResourcesUsed(Player * entity, int value)
{
	//TODO
}

void GameManager::NextState() //Only for the mulligan
{
	switch (state)
	{
	case NO_GAMESTATE:
		state = CREATING;
		HS_LOG("Creating game");
		break;
	case CREATING:
		MSG_ERROR("----------------------------------------");
		HS_LOG("%s", p_op->hero->name.data());
		HS_LOG("    %s", p_op->hero->playerClass.data());
		MSG_WARNING("----------------------------------------");
		HS_LOG("        V.S");
		MSG_WARNING("----------------------------------------");
		HS_LOG("%s", p_ai->hero->name.data());
		HS_LOG("    %s", p_ai->hero->playerClass.data());
		MSG_ERROR("----------------------------------------");
		break;
	}
}

void GameManager::Reset()
{
	state = GameState::NO_GAMESTATE;
	mulligan_cards.clear();

	if (p_ai != nullptr)
	{
		delete p_ai;
		p_ai = nullptr;
	}

	if (p_op != nullptr)
	{
		delete p_op;
		p_op = nullptr;
	}
}
