#include "LogParser.h"
#include <stdio.h>
#include "ConsoleMsgs.h"
#include "CardsDB.h"
#include "Application.h"
#include "CardTypes.h"
#include "GameManager.h"

using namespace CardTypes;

LogParser::LogParser()
{
}

LogParser::~LogParser()
{
}

void LogParser::Update() //Doesn't work
{
	log_path = "C:\\Program Files (x86)\\Hearthstone\\Logs\\Power.log";
	FILE* p_file = fopen(log_path.data(), "rb");
	if (p_file == NULL)
	{
		MSG_ERROR("Error openng the file %s", log_path);
		return;
	}

	//Obtain the size:
	fseek(p_file, 0, SEEK_END);
	long f_size = ftell(p_file);

	if (f_size == file_pointer || f_size == 0) //No updates
	{
		fclose(p_file);
		return;
	}

	long new_size = f_size - file_pointer;
	char* buf = new char[new_size*sizeof(char)];
	std::string buf_s;
	buf_s.reserve(new_size);

	fseek(p_file, file_pointer, SEEK_SET);
	size_t ret_size = fread(&buf[0], 1, new_size, p_file);
	if (ret_size != new_size)
	{
		MSG_ERROR("Error reading the file %s", log_path);
	}
	else
	{
		buf_s = buf;
		//Print line by line
		std::string end_line = "\r\n";
		size_t sb_pointer = 0;
		size_t se_pointer = 0;

		while (se_pointer != std::string::npos)
		{
			se_pointer = buf_s.find(end_line, sb_pointer);
			if (se_pointer != std::string::npos)
			{
				std::string line = buf_s.substr(sb_pointer, se_pointer - sb_pointer);
				ExtractInfo(line);
				sb_pointer = se_pointer+2;
			}
		}
	}
	file_pointer = f_size;
	if (buf)
		delete[] buf;
	fclose(p_file);
}

void LogParser::ExtractInfo(const std::string & line)
{
	//Starts at char[19] - char[24]
	//Get Output type: GameState / PowerTaskList /PowerProcessor
	char c = line[19];
	switch (c)
	{
	case 'G': //GameState
		ExtractGameState(line);
		break;
	case 'P':
		char c2 = line[24];
		switch (c2)
		{
		case 'T': //PowerTaskList
			break;
		case 'P'://PowerProcessor
			break;
		}
		break;
	}
}

void LogParser::ExtractGameState(const std::string & line) //I found out this can be replaced for a switch. Too lazy to change it now.
{
	//D 16:53:25.6738332 GameState. [29]
	//DebugPrintPower - DebugPrintPowerList - DebugPrintOption - DebugPrintEntitiesChosen - SendChoices - SendOption
	char debug_or_send = line[29];
	if (debug_or_send == 'D') //Debug
	{//D 16:53:25.6738332 GameState.DebugPrint [39]
		char power_option_entities = line[39];
		if (power_option_entities == 'P') //Power
		{//D 16:53:25.6738332 GameState.DebugPrintPower [44]
			char list_or_not = line[44];
			if (list_or_not == 'L') //DebugPrintPowerList
			{
				ExtractDebugPrintPowerList(line);
			}
			else if (list_or_not == '(') //DebugPrintPower
			{
				ExtractDebugPrintPower(line);
			}
		}
		else if (power_option_entities == 'O') //DebugPrintOption
		{

		}
		else if (power_option_entities == 'E') //DebugPrintEntitiesChosen
		{

		}
	}
	else if (debug_or_send == 'S') //Send
	{//D 16:53:25.6738332 GameState.Send [33]
		char choices_option = line[33];
		if (choices_option == 'C') //SendChoices
		{

		}
		else if (choices_option == 'O') //SendOption
		{

		}
	}
}

void LogParser::ExtractDebugPrintPowerList(const std::string & line)
{//D 16:53:05.8892265 GameState.DebugPrintPowerList() - Count= [59]
	power_list_count = atoi(line.substr(59, line.length() - 59).data()); //Useless for now
}

void LogParser::ExtractDebugPrintPower(const std::string & line)
{ //D 16:53:05.8922419 GameState.DebugPrintPower() - [49]
	//CREATE_GAME - BLOCK_START - BLOCK_END - TAG_CHANGE - FULL_ENTITY
	char create_block_tag_full = line[49];
	switch (create_block_tag_full)
	{
	case 'C': //CREATE_GAME
		App->game_manager->StartGame();
		gdpps_count_to_reset = 2; //Find 2 players
		GDPPS = GameDebugPrintPowerState::PLAYERS_ID;
	break;
	case 'B': //BLOCK
	{	char b = line[55];
		switch (b)
		{
		case 'S': //BLOCK_START
			break;
		case 'E': //BLOCK_END
			break;
		}
	}
	break;
	case 'T': //TAG_CHANGE
	break;
	case 'F': //FULL_ENTITY
		ExtractDebugPrintPowerFullEntity(line);
	break;
	case ' ': //Indexed parameters [T]TAG_CHANGE - [S]SHOW_ENTITY - [F]FULL_ENTITY - [P]PLAYER - [M]META_DATA
		switch (GDPPS)
		{
		case PLAYERS_ID:
			ExtractPlayersIds(line);
			break;
		case HERO_TYPE:
			ExtractHeroType(line);
			break;
		}
	break;
	}
}

void LogParser::ExtractPlayersIds(const std::string & line)
{ //D 19:09 : 07.8072477 GameState.DebugPrintPower() -     [53]
	//Player Entity [P]
	char c = line[53];
	if (c == 'P')
	{ //Player EntityID=2 PlayerID=1 GameAccountId=[hi=144115198130930503 lo=20525159]
		int entitiy_id = (int)line[69] - 48; //Useful maybe? Not saving right now
		int player_id = (int)line[80] - 48;
		size_t found = line.find("20525159", 80);
		if (found == std::string::npos)
		{
			App->game_manager->SetPlayer(player_id, PlayerType::OPPONENT);
		}
		else
		{
			App->game_manager->SetPlayer(player_id, PlayerType::YOU);
		}
		--gdpps_count_to_reset;
		if (gdpps_count_to_reset == 0)
			GDPPS = GameDebugPrintPowerState::GDPPS_NONE;
	}
}

void LogParser::ExtractHeroType(const std::string & line)
{
	//D 19:09 : 07.8072477 GameState.DebugPrintPower() -     [53]
	//tag [t]
	char c = line[53];
	if (c == 't')
	{ //D 19:09 : 08.0107638 GameState.DebugPrintPower() - tag=CONTROLLER value=2
		size_t found = line.find("CONTROLLER");
		if (found != std::string::npos)
		{
			int player_id = (int)(line[found + 17]) - 48;
			App->game_manager->SetHero(player_id, tmp_hero);
			tmp_hero.clear();
			--gdpps_count_to_reset;
			if (gdpps_count_to_reset == 0)
				GDPPS = GameDebugPrintPowerState::GDPPS_NONE;
		}	
	}
}

void LogParser::ExtractDebugPrintPowerFullEntity(const std::string & line)
{
	size_t card_id_pos = line.find_last_of('=');
	if (card_id_pos == std::string::npos) 
		return; 
	else 
		++card_id_pos;
	if (line.length() > card_id_pos)
	{
		std::string card_id = line.substr(card_id_pos, line.length() - card_id_pos);
		std::string card_type = App->cards_db->GetType(card_id);
		if (card_type == HERO)
		{
			tmp_hero = card_id;
			GDPPS = GameDebugPrintPowerState::HERO_TYPE;
			gdpps_count_to_reset = 1;
		}
	}
}


