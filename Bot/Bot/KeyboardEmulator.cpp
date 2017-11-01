#include "KeyboardEmulator.h"

#include <Windows.h>


KeyboardEmulator::KeyboardEmulator()
{
}

KeyboardEmulator::~KeyboardEmulator()
{
}

void KeyboardEmulator::InitWrite(const string & text)
{
	s_id = 0;
	this->text = text;
	length = text.length();
}

bool KeyboardEmulator::Write()
{
	if (key_down == false)
	{
		SendKeyInput(text[s_id], false);
		key_down = true;
	}
	else
	{
		SendKeyInput(text[s_id], true);
		++s_id;
		key_down = false;
		if (s_id == length)
			return true;
	}
	return false;
}

void KeyboardEmulator::SendKeyInput(char c, bool up)
{
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VkKeyScanEx(c, NULL);
	if(up)
		input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}
