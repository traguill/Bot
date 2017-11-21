#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <string>
#include <vector>

class Player;

enum GameState
{
	NO_GAMESTATE,
	CREATING,
	BEGIN_MULLIGAN,
	MAIN_READY,
	MAIN_START_TRIGGERS,
	MAIN_START,
	MAIN_ACTION,
	MAIN_END,
	MAIN_CLEANUP,
	MAIN_NEXT,
	FINAL_WRAPUP,
	FINAL_GAMEOVER,
	COMPLETE
};

enum PlayerType
{
	NO_PLAYERTYPE,
	OPPONENT,
	YOU
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	void StartGame();
	void SetPlayer(int num, PlayerType type);
	void SetHero(int num, const std::string& hero);
	void SetNickname(int num, const std::string& nick);
	void InsertMulliganCard(const std::string& card_id);

	void CardDrawn(const std::string& card_id);

	void ChangeState(GameState new_state);

	//Utilities
	Player* StringToPlayer(const std::string& line, size_t start);

	//InGame
	void ResourcesUsed(Player* entity, int value);

private:
	void NextState();

	void Reset();

private:
	GameState state = GameState::NO_GAMESTATE;

	std::vector<std::string> mulligan_cards;

	Player* p_ai = nullptr; //AI-You
	Player* p_op = nullptr; //Opponent

	Player* current_player = nullptr; //Who has the turn

public:
	int ai_id = 0; //Id player of the AI. quick access
};

#endif // !__GAMEMANAGER_H__
