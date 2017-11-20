#ifndef __LOGPARSER_H__
#define __LOGPARSER_H__

#include <string>

enum GameDebugPrintPowerState
{
	GDPPS_NONE,
	PLAYERS_ID,
	HERO_TYPE
};


class LogParser
{
public:
	LogParser();
	~LogParser();

	void Update();

private:

	void ExtractInfo(const std::string& line);

	//Types
	void ExtractGameState(const std::string& line);

	//SubGameTypes
	void ExtractDebugPrintPowerList(const std::string& line);
	void ExtractDebugPrintPower(const std::string& line);

	void ExtractPlayersIds(const std::string& line);
	void ExtractHeroType(const std::string& line);
	void ExtractDebugPrintPowerFullEntity(const std::string& line);

private:

	std::string log_path;
	long file_pointer = 0;

	int power_list_count = 0; //Useless

	GameDebugPrintPowerState GDPPS = GameDebugPrintPowerState::GDPPS_NONE;
	int gdpps_count_to_reset = 0;

	std::string tmp_hero;
};
#endif // !__LOGPARSER_H__
