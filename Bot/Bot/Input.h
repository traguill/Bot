#ifndef __INPUT_H__
#define __INPUT_H__

#include <vector>
#include "Point.h"

#define MOUSE_SCREEN_SIZE 65535.0f

enum BUTTON_STATE
{
	IDLE,
	DOWN,
	REPEAT,
	UP
};

enum MOUSE_STATE
{
	MOUSE_IDLE,
	MOUSE_MOVE
};

struct ButtonStateTMP
{
	BUTTON_STATE* src = nullptr;
	BUTTON_STATE new_state;

	ButtonStateTMP();
	ButtonStateTMP(BUTTON_STATE* src, BUTTON_STATE new_state);
};

class Input
{
public:
	Input();
	~Input();

	bool Update();

	void MouseButton(int id, BUTTON_STATE state); // 0-Left 1-Middle 2-Right
	void MouseMove(int x, int y);

	BUTTON_STATE GetMouseButton(int id)const; //0-Left 1-Middle 2-Right
	Point<int> GetMousePosition()const;

	void MouseToScreenUnits(int&x, int& y)const;

private:
	void UpdateButtonState(BUTTON_STATE& button);

private:
	MOUSE_STATE mouse_state = MOUSE_STATE::MOUSE_IDLE;
	BUTTON_STATE mouse_lb = BUTTON_STATE::IDLE;

	std::vector<ButtonStateTMP> input_to_process;

	Point<int> mouse_position;
	Point<int> screen_size;
};
#endif // !__INPUT_H__
