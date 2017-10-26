#include <iostream>
#include <string>

#include <Windows.h>
#include <thread>
#include <string>

#include "MouseEmulator.h"
#include "Timer.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Performance.h"

using namespace std;

Timer* timer = nullptr;
Application* App = nullptr;
Console* console = nullptr;
Performance* performance = nullptr;


HHOOK MouseHook;

LRESULT __stdcall MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (App && App->input)
	{
		if (wParam == WM_LBUTTONDOWN)
		{
			App->input->MouseButton(0, BUTTON_STATE::DOWN);
		}
		if (wParam == WM_LBUTTONUP)
		{
			App->input->MouseButton(0, BUTTON_STATE::UP);
		}
		if (wParam == WM_MOUSEMOVE)
		{
			//App->input->MouseMove(LOWORD(lParam), HIWORD(lParam));
		}
	}
	return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}

void InputMethod()
{
	string msg;
	bool quit = false;
	while (!quit)
	{
		console->PrintHeader();
		printf("> ");
		getline(cin, msg);
		console->Execute(msg.data());
		printf("\n");
		quit = console->quit_request;
	}

	printf("Input thread exit\n");
}

int  main(int argc, char*argv[])
{
	MSG msg;
	
	//performance = new Performance();
	//performance->Init();

	timer = new Timer();
	timer->Init();

	App = new Application(argv[0]);
	App->Init();

	console = new Console();
	console->Start();
	
	std::thread t = std::thread(InputMethod);
	t.detach();
	

	//MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
	bool quit = false;
	while (!quit)
	{
		float dt = timer->Update();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		quit = !App->Update(dt);
	}

	//UnhookWindowsHookEx(MouseHook);
	
	delete console;
	delete App;
	delete timer;
	//delete performance;
	printf("Application exits\n");
	Sleep(500);

	return 0;
}