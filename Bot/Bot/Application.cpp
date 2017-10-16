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

bool Application::Menu()
{
	return true;

	bool succeed = true;
	bool ret = false;

	do
	{
		cout << "(1)- Create a routine\n";
		cout << "(2)- Play last routine\n";
		cout << "(3)- Quit\n";

		int option;
		cin >> option;

		switch (option)
		{
		case 1:
			ret = CreateNewRoutine();
			break;
		case 2:
			break;
		case 3:
			ret = false; //Quit (not need it if ret is false by default)
			break;
		default:
			cout << "Invalid option selected!\n";
			succeed = false;
			break;
		}
	} while (!succeed);


	return ret;
}

bool Application::CreateNewRoutine()
{
	state = RECORDING_ROUTINE;



	return true;
}

void Quit(const vector<string>* args)
{
	App->quit_request = true; //Stop main thread
	console->quit_request = true;
}
