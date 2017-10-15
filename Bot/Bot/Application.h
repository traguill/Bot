#ifndef __APLICATION_H__
#define __APLICATION_H__

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
private:

	APP_STATE state = MENU;

	

};

extern Application* App;
#endif // !__APLICATION_H__
