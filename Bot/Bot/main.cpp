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

using namespace std;

Timer* timer = nullptr;
Application* App = nullptr;
bool quit = false;
Console* console = nullptr;

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
	cout << "Joinigng input\n";

	bool end = false;

	string msg;
	while (!end)
	{
		getline(cin, msg);
		
		if (msg.compare("q") == 0)
		{
			cout << "Exiting\n";
			end = true;
		}
		else
		{
			console->Execute(msg.data());
		}
	}

	cout << "Ending the input thread\n";
	quit = true;
}

int  main()
{


	MSG msg;

	timer = new Timer();
	timer->Init();

	App = new Application();

	console = new Console();
	
	std::thread t = std::thread(InputMethod);
	cout << "Starting input thread\n";
	t.join();
	

	//MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);

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
	cout << "EXIT";
	Sleep(500);

	return 0;
}


//Mouse hook to get the external input of the mouse

//HHOOK MouseHook;

/*LRESULT __stdcall MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{

if (wParam == WM_LBUTTONUP)
{

}

return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}*/
//MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
//UnhookWindowsHookEx(MouseHook);