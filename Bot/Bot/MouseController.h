#ifndef __MOUSECONTROLLER_H__
#define __MOUSECONTROLLER_H__

class MouseEmulator;

class MouseController
{
public:
	MouseController();
	~MouseController();

	bool Update();

private:
	MouseEmulator* mouse = nullptr;

};
#endif // !__MOUSECONTROLLER_H__

