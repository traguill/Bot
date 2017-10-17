#include "Application.h"

#include <iostream>

#include "Editor.h"
#include "Input.h"
#include "Console.h"

using namespace std;

Application::Application()
{
	editor = new Editor();
	input = new Input();
}

Application::~Application()
{
	delete input;
	delete editor;
}

bool Application::Update(float dt)
{
	if (quit_request)
		return false;

	bool ret = false;

	input->Update();
	ret = editor->Update();

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

bool Application::Menu()
{
	return true;
}

void Quit(const vector<string>* args)
{
	App->quit_request = true; //Stop main thread
	console->quit_request = true;
}
