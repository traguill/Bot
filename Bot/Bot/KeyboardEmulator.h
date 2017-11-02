#ifndef __KEYBOARD_EMULATOR_H__
#define __KEYBOARD_EMULATOR_H__

#include <string>

using namespace std;

enum SPECIAL_KEY
{
	ENTER
};

class KeyboardEmulator
{
public:
	KeyboardEmulator();
	~KeyboardEmulator();

	void InitWrite(const string& text);

	bool Write(); //Returns true when it's done

	void InitKey(SPECIAL_KEY key);
	bool PressKey();

private:
	void SendKeyInput(char c, bool up);
	void SendSPKeyInput(char c, bool up);

private:
	string text;
	unsigned int s_id = 0;
	bool key_down = false;
	unsigned int length;

	char sp_key;

	//TODO: Humanize. Between 0.315 and 0.3s per character.
};
#endif // !__KEYBOARD_EMULATOR_H__
