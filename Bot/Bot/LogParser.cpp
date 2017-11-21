#include "LogParser.h"
#include <stdio.h>
#include "ConsoleMsgs.h"
#include "CardsDB.h"
#include "Application.h"
#include "CardTypes.h"

using namespace CardTypes;

LogParser::LogParser()
{
	LoadGameStates();
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
				line = buf_s.substr(sb_pointer, se_pointer - sb_pointer);
				++line_count;
				ExtractInfo();
				sb_pointer = se_pointer+2;
			}
		}
	}
	file_pointer = f_size;
	if (buf)
		delete[] buf;
	fclose(p_file);
}

void LogParser::ExtractInfo()
{
	//Starts at char[19] - char[24]
	//Get Output type: GameState / PowerTaskList /PowerProcessor
	char c = line[19];
	switch (c)
	{
	case 'G': //GameState
		ExtractGameState();
		break;
	case 'P': //This ones are useless for now
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

void LogParser::ExtractGameState() //I found out this can be replaced for a switch. Too lazy to change it now.
{
	//D 16:53:25.6738332 GameState. [29]
	//DebugPrintPower - DebugPrintPowerList - DebugPrintOption - DebugPrintEntitiesChosen -DebugPrintEntityChoices - SendChoices - SendOption
	char debug_or_send = line[29];
	if (debug_or_send == 'D') //Debug
	{//D 16:53:25.6738332 GameState.DebugPrint [39]
		char power_option_entities = line[39];
		if (power_option_entities == 'P') //Power
		{//D 16:53:25.6738332 GameState.DebugPrintPower [44]
			char list_or_not = line[44];
			if (list_or_not == 'L') //DebugPrintPowerList
			{
				ExtractDebugPrintPowerList();
			}
			else if (list_or_not == '(') //DebugPrintPower
			{
				ExtractDebugPrintPower();
			}
		}
		else if (power_option_entities == 'O') //DebugPrintOption
		{

		}
		else if (power_option_entities == 'E') //DebugPrint EntitiesChosen/EntityChoices
		{ //Discover, battlecry targets, spells targets. Everything that forces a player to make a decision
			size_t is_choices = line.find("Choices");
			if (is_choices != std::string::npos) //Choices
			{
				ExtractChoices();
			}
			else //Chosen
			{
				size_t is_chosen = line.find("Chosen");
				if (is_chosen != std::string::npos)
				{
					//TODO: Chosen option register
				}
			}
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

void LogParser::ExtractDebugPrintPowerList()
{//D 16:53:05.8892265 GameState.DebugPrintPowerList() - Count= [59]
	power_list_count = atoi(line.substr(59, line.length() - 59).data()); //Useless for now
}

void LogParser::ExtractDebugPrintPower()
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
	{	
		char b = line[55]; //TODO:Check BlockType trigger->state -play = card deaths->minions fallen
		switch (b)
		{
		case 'S': //BLOCK_START
		{	GDPPS = GameDebugPrintPowerState::BLOCK;
			size_t found = line.find("BlockType=TRIGGER");
			if (found != std::string::npos) { block_type = BlockType::B_TRIGGER; break; }
			found = line.find("BlockType=PLAY");
			if (found != std::string::npos) { block_type = BlockType::B_PLAY; break; }
			found = line.find("BlockType=DEATHS");
			if (found != std::string::npos) { block_type = BlockType::B_DEATHS; break; }
		}
			break;
		case 'E': //BLOCK_END
			GDPPS = GameDebugPrintPowerState::GDPPS_NONE;
			break;
		}
	}
	break;
	case 'T': //TAG_CHANGE
	{
		size_t found = line.find("GameEntity"); 
		if (found != std::string::npos)
			ExtractGameStep();
	}
	break;
	case 'F': //FULL_ENTITY
		ExtractDebugPrintPowerFullEntity();
	break;
	case ' ':
		switch (GDPPS)
		{
		case PLAYERS_ID:
			ExtractPlayersIds();
			break;
		case HERO_TYPE:
			ExtractHeroType();
			break;
		case BLOCK:
			ExtractBlock();
			break;
		}
	break;
	}
}

void LogParser::ExtractPlayersIds()
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

void LogParser::ExtractHeroType()
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

void LogParser::ExtractDebugPrintPowerFullEntity()
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

void LogParser::ExtractBlock()
{ 
	switch (block_type)
	{
	case B_TRIGGER:
	{	//Inside block - SHOW_ENTITY - TAG_CHANGE
		char c = line[53];
		switch (c)
		{
		case 'S':
			ExtractShowEntity();
			break;
		case 'T':
			switch (current_game_state)
			{
			case MAIN_READY:
				//Info about resources spent, cards num_turns_in_play, num_cards_draw. Not need it righ now.
				break;
			case MAIN_START_TRIGGERS:
				//Info about triggers at the beginning of the turn. Research
				break;
			case MAIN_START:
				//card draws this turn, minions killed etc..
				break;
			case MAIN_ACTION:
				//MSG_INFO("(%i) - %s", line_count, line.data() + 53);
				//cards played, attacks, etc.
				break;
			case MAIN_END:
				//MSG_INFO("(%i) - %s", line_count, line.data() + 53); //end turn effects. ARMS_DEALING (+1/+1)
				break;
			case MAIN_CLEANUP:
				//MSG_INFO("(%i) - %s", line_count, line.data() + 53);
				//update just played cards, exhausted secrets or weapons (not in use)
				break;
			case MAIN_NEXT:
				//MSG_INFO("(%i) - %s", line_count, line.data() + 53);
				//Change current player. +1 current player turn
				break;
			}
			break;
			}
		break;
	}
	case B_PLAY:
		ExtractBlockPlay();
		break;
	case B_DEATHS:
		break;
	}
	
}

void LogParser::ExtractShowEntity()
{
	//D 23:39:03.3971037 PowerTaskList.DebugPrintPower() -     SHOW_ENTITY S[53]
	size_t id = line.find("CardID=");

	id += 7;
	std::string card_id = line.substr(id);

	//zone=DECK zonePos=0 player=1
	size_t player_pos = line.find("player");
	if (player_pos == std::string::npos) //Mulligan cards
	{
		App->game_manager->InsertMulliganCard(card_id);
	}
	else //player=YOU ->Card drawn
	{
		int player = (int)(line[player_pos + 7] - 48);
		if (App->game_manager->ai_id == player)		
			App->game_manager->CardDrawn(card_id);
	}
}

void LogParser::ExtractBlockPlay()
{
	//TAG_CHANGE or BLOCK_START
	size_t sub_type = line.find("TAG_CHANGE");
	if (sub_type != std::string::npos) //Tag change
	{
		size_t entity = line.find("Entity=") + 7;
		if (line[entity] == '[')
			ExtractBlockPlayCard();
		else
			ExtractBlockPlayPlayer(entity);
	}
	else
	{//block start -> effects battlecries
		sub_type = line.find("BLOCK_START");
		if (sub_type != std::string::npos)
		{

		}
	}
}

void LogParser::ExtractBlockPlayPlayer(size_t name_pos)
{
	size_t val_pos = line.find("value=") + 6;
	std::string val_s = line.substr(val_pos, line.length() - 1 - val_pos);
	int value = atoi(val_s.data());
	Player* entity = App->game_manager->StringToPlayer(line, name_pos);
	//tag: RESOURCES_USED/NUM_RESOURCES_SPENT_THIS_GAME/NUM_CARDS_PLAYED_THIS_TURN/NUM_MINIONS_PLAYED_THIS_TURN
	size_t found = line.find("RESOURCES_USED", name_pos);
	if (found != std::string::npos) App->game_manager->ResourcesUsed(entity, value);
	else
	{

	}
}

void LogParser::ExtractBlockPlayCard()
{
}

void LogParser::ExtractGameStep()
{
	size_t value_pos = line.find_last_of('=') + 1;
	std::string step = line.substr(value_pos, line.length() - value_pos - 1); //Remove the space at the end of the line

	std::map<std::string, GameState>::iterator it = state_conversor.find(step);
	if (it != state_conversor.end())
	{
		current_game_state = (*it).second;
		App->game_manager->ChangeState(current_game_state);
	}
}

void LogParser::ExtractChoices()
{
	//Sort by type: MULLIGAN, TODO: Research more types
	size_t type_pos = line.find("ChoiceType=") + 11;
	size_t end_type_pos = line.find_first_of(' ', type_pos);
	std::string type = line.substr(type_pos, end_type_pos - type_pos);

	if (type.compare("MULLIGAN") == 0) ExtractChoicesMulligan();
}

void LogParser::ExtractChoicesMulligan()
{// example: D 11:17:44.4063940 GameState.DebugPrintEntityChoices() - id=1 Player=PlayerNickName TaskList=4 ChoiceType=MULLIGAN CountMin=0 CountMax=3
	size_t id_pos = line.find("id=") + 3;
	int id = (int)line[id_pos] - 48;
	size_t player_pos = line.find("Player=") + 7;
	size_t player_end = line.find_first_of(' ', player_pos);
	std::string nickname = line.substr(player_pos, player_end - player_pos);
	App->game_manager->SetNickname(id, nickname);
}

void LogParser::LoadGameStates()
{
	state_conversor.insert(pair<std::string, GameState>("BEGIN_MULLIGAN", BEGIN_MULLIGAN));
	state_conversor.insert(pair<std::string, GameState>("MAIN_READY", MAIN_READY));
	state_conversor.insert(pair<std::string, GameState>("MAIN_START_TRIGGERS", MAIN_START_TRIGGERS));
	state_conversor.insert(pair<std::string, GameState>("MAIN_START", MAIN_START));
	state_conversor.insert(pair<std::string, GameState>("MAIN_ACTION", MAIN_ACTION));
	state_conversor.insert(pair<std::string, GameState>("MAIN_END", MAIN_END));
	state_conversor.insert(pair<std::string, GameState>("MAIN_CLEANUP", MAIN_CLEANUP));
	state_conversor.insert(pair<std::string, GameState>("MAIN_NEXT", MAIN_NEXT));
	state_conversor.insert(pair<std::string, GameState>("FINAL_WRAPUP", FINAL_WRAPUP));
	state_conversor.insert(pair<std::string, GameState>("FINAL_GAMEOVER", FINAL_GAMEOVER));
	state_conversor.insert(pair<std::string, GameState>("COMPLETE", COMPLETE));
}


