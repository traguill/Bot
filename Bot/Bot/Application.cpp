#include "Application.h"

#include <iostream>

#include "Editor.h"
#include "Input.h"
#include "Console.h"
#include "ModuleFileSystem.h"
#include "Random.h"
#include "MouseEmulator.h"
#include "KeyboardEmulator.h"
#include "LogParser.h";
#include "CardsDB.h"

using namespace std;

Application::Application(const char* argv0) : argv0(argv0)
{
	rnd = new Random();
	file_system = new ModuleFileSystem();
	editor = new Editor();
	input = new Input();
	mouse = new MouseEmulator();
	keyboard = new KeyboardEmulator();
	log_parser = new LogParser();
	cards_db = new CardsDB();
}

Application::~Application()
{
	SetConsoleTextAttribute(h_console, csbi.wAttributes);

	delete cards_db;
	delete log_parser;
	delete keyboard;
	delete mouse;
	delete input;
	delete editor;
	delete file_system;
	delete rnd;
}

void Application::Init()
{
	h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h_console, &csbi);

	//Make the font smaller
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(h_console, false, &font);
	font.dwFontSize.X = 14;
	font.dwFontSize.Y = 14;
	SetCurrentConsoleFontEx(h_console, false, &font);

	file_system->Init();
	editor->Init();

	cards_db->Init();
	
}

bool Application::Update(float dt)
{
	this->dt = dt;

	if (quit_request)
		return false;

	bool ret = false;

	input->Update();
	ret = editor->Update();

	log_parser->Update();

	return ret;
}

void Application::BlockConsole()
{
	if (console_locked == false)
	{
		console_locked = true;
		unique_lock<mutex>lck(mtx);
		cv.wait(lck);
	}
}

void Application::UnblockConsole()
{
	if (console_locked)
	{
		console_locked = false;
		unique_lock<mutex>lck(mtx);
		cv.notify_all();
	}
}

void Quit(const vector<string>* args)
{
	App->quit_request = true; //Stop main thread
	console->quit_request = true;
}
