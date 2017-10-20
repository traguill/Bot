#include "MouseController.h"
#include "MouseEmulator.h"
#include "Application.h"
#include "Editor.h"

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
	ret = mouse->Move(App->dt); 

	if (ret)
		mouse->Clear();

	return ret;
}

void MouseController::GoTo(const Point<int>& dst, float delay)
{
	mouse->AddPoint(dst.x, dst.y);
	mouse->InitMovement(delay);

	App->editor->ChangeState(EDITOR_STATE::MOUSE_MOVING);
}
