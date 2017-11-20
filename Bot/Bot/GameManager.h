#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <string>

enum GameState
{
	NO_GAMESTATE,
	CREATING,
	MULLIGAN,
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
private:
	void NextState();
private:
	GameState state = GameState::NO_GAMESTATE;

	PlayerType player1 = PlayerType::NO_PLAYERTYPE;
	PlayerType player2 = PlayerType::NO_PLAYERTYPE;

	std::string hero1;
	std::string hero2;
};

#endif // !__GAMEMANAGER_H__
