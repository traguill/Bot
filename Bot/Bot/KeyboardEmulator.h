#ifndef __KEYBOARD_EMULATOR_H__
#define __KEYBOARD_EMULATOR_H__

#include <string>

using namespace std;

class KeyboardEmulator
{
public:
	KeyboardEmulator();
	~KeyboardEmulator();

	void InitWrite(const string& text);

	bool Write(); //Returns true when it's done

private:
	void SendKeyInput(char c, bool up);

private:
	string text;
	unsigned int s_id = 0;
	bool key_down = false;
	unsigned int length;

	//TODO: Humanize. Between 0.315 and 0.3s per character.
};
#endif // !__KEYBOARD_EMULATOR_H__
