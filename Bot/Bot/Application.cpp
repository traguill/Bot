#include "Application.h"

#include <iostream>

#include "Editor.h"
#include "Input.h"
#include "Console.h"
#include "ModuleFileSystem.h"
#include "Random.h"

using namespace std;

Application::Application(const char* argv0) : argv0(argv0)
{
	rnd = new Random();
	file_system = new ModuleFileSystem();
	editor = new Editor();
	input = new Input();
}

Application::~Application()
{
	delete input;
	delete editor;
	delete file_system;
	delete rnd;
}

void Application::Init()
{
	file_system->Init();
	editor->Init();
}

bool Application::Update(float dt)
{
	this->dt = dt;

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

void Quit(const vector<string>* args)
{
	App->quit_request = true; //Stop main thread
	console->quit_request = true;
}
