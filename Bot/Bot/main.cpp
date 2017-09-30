

#include <iostream>
#include <string>

#include <Windows.h>

#include "MouseEmulator.h"
#include "Timer.h"

using namespace std;

HHOOK MouseHook;

int num_clicks;
int clicks_counter;

bool start_moving;

MouseEmulator mouse;
Timer* timer = nullptr;

LRESULT __stdcall MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (wParam == WM_LBUTTONUP)
	{
		if (start_moving == false)
		{
			POINT p;
			GetCursorPos(&p);

			mouse.AddPoint(p.x, p.y);

			++clicks_counter;

			if (clicks_counter == num_clicks)
			{
				mouse.InitMovement();
				start_moving = true;
			}
		}
	}

	return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}

int  main()
{
	bool quit = false;

	clicks_counter = 0;
	num_clicks = 0;
	start_moving = false;

	cout << "Introduce the number of clicks: \n";
	cin >> num_clicks;

	mouse.speed = 800;
	mouse.dst_threshold = 20;


	MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
	MSG msg;

	timer = new Timer();
	timer->Init();

	while (!quit)
	{
		float dt = timer->Update();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (start_moving)
		{
			if (mouse.Move(dt))
				quit = true;
		}
	}

	UnhookWindowsHookEx(MouseHook);

	delete timer;
	cout << "EXIT";
	Sleep(2000);

	return 0;
}