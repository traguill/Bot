#include "MouseController.h"
#include "MouseEmulator.h"

MouseController::MouseController()
{
	mouse = new MouseEmulator();
}

MouseController::~MouseController()
{
	delete mouse;
}

bool MouseController::Update()
{
	bool ret;
	ret = mouse->Move(0.16f); //TODO pass dt;

	if (ret)
		mouse->Clear();

	return ret;
}
