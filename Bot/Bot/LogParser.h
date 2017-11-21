#ifndef __LOGPARSER_H__
#define __LOGPARSER_H__

#include <string>
#include <map>
#include "GameManager.h"

enum GameDebugPrintPowerState
{
	GDPPS_NONE,
	PLAYERS_ID,
	HERO_TYPE,
	BLOCK
};

enum BlockType
{
	B_TRIGGER,
	B_PLAY,
	B_DEATHS
};


class LogParser
{
public:
	LogParser();
	~LogParser();

	void Update();

private:

	void ExtractInfo();

	//Types
	void ExtractGameState();

	//SubGameTypes
	void ExtractDebugPrintPowerList();
	void ExtractDebugPrintPower();

	void ExtractPlayersIds();
	void ExtractHeroType();
	void ExtractDebugPrintPowerFullEntity();

	//BLOCK
	void ExtractBlock();
		void ExtractShowEntity();
		void ExtractBlockPlay();
			void ExtractBlockPlayPlayer(size_t name_pos);
			void ExtractBlockPlayCard();

	void ExtractGameStep();

	//EntityChoices
	void ExtractChoices();

	//ChoicesTypes
	void ExtractChoicesMulligan();

	//Init
	void LoadGameStates();

private:

	std::string log_path;
	long file_pointer = 0;
	//Debug
	int line_count = 0;

	std::string line;

	int power_list_count = 0; //Useless

	GameDebugPrintPowerState GDPPS = GameDebugPrintPowerState::GDPPS_NONE;
	int gdpps_count_to_reset = 0;

	std::string tmp_hero;

	BlockType block_type;

	std::map<std::string, GameState> state_conversor;
	GameState current_game_state = GameState::NO_GAMESTATE;
};
#endif // !__LOGPARSER_H__
