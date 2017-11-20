#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <windows.h>

using namespace std;

class Editor;
class Input;
class ModuleFileSystem;
class Random;
class MouseEmulator;
class KeyboardEmulator;
class LogParser;
class CardsDB;
class GameManager;

class Application
{
public:
	Application(const char* argv0);
	~Application();

	void Init();

	bool Update(float dt);

	//Blocks console input until Unblock() is called
	void BlockConsole();
	void UnblockConsole();


public:
	Random* rnd = nullptr;
	Editor* editor = nullptr;
	Input* input = nullptr;
	ModuleFileSystem* file_system = nullptr;
	MouseEmulator* mouse = nullptr;
	KeyboardEmulator* keyboard = nullptr;
	LogParser* log_parser = nullptr;
	CardsDB* cards_db = nullptr;
	GameManager* game_manager = nullptr;
	bool quit_request = false;

	mutex mtx;
	condition_variable cv;

	const char* argv0; 

	float dt = 0.0f;

	//Console
	HANDLE h_console; //Used to change the console color & font size
	CONSOLE_SCREEN_BUFFER_INFO csbi; //To restore the initial console values

private:
	bool console_locked = false;
};

//Console commands
void Quit(const vector<string>* args);

extern Application* App;
#endif // !__APLICATION_H__
