#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <vector>
#include <string>

using namespace std;

class Editor;
class Input;

enum APP_STATE
{
	MENU,
	RECORDING_ROUTINE,
	ROUTINE
};

class Application
{
public:
	Application();
	~Application();

	bool Update(float dt);

private:

	bool Menu();
	bool CreateNewRoutine();

public:
	Editor* editor = nullptr;
	Input* input = nullptr;
	bool quit_request = false;

private:

	APP_STATE state = MENU;
};

//Console commands
void Quit(const vector<string>* args);

extern Application* App;
#endif // !__APLICATION_H__
