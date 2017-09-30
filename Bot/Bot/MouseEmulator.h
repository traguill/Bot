#ifndef __MOUSEEMULATOR_H__
#define __MOUSEEMULATOR_H__

//Saves an array of positions
//Handles the movement of the mouse and the clicks

#include <vector>

#include "Point.h"

using namespace std;

#define MOUSE_SCREEN_SIZE 65535.0f

class MouseEmulator
{
public:
	MouseEmulator();
	~MouseEmulator();

	void AddPoint(int x, int y);

	//Debug
	void PrintPoints();

	void InitMovement(); //Resets all the movement variables
	bool Move(float dt); //Return true when all points have been visited once. 

private:

	bool ComputeDst();
	bool RecomputeDst();

	void LeftClick();
	void LeftClickUp();
	

	void ScreenToMouseUnits(int& x, int& y)const;

public:

	float speed = 0.0f;
	float dst_threshold = 0.0f; //Radius of threshold for the mouse to arrive to it's dst
private:

	vector<Point<int>> points;

	//Movement
	Point<float> step; //Movement step
	Point<float> accumulated_step; //Mouse can only move in integer units. Sometimes step is smaller than 1. Save the amount of displacement until is bigger than 1. 
	Point<int> dst;
	bool has_dst = false;
	unsigned int point_id = 0;

	Point<int> current_mouse_position;

	Point<int> screen_size;

	bool mouse_down = false;
	float countdown_mouse = 0.0f; //Counter for the time the left button is down

	bool can_start = false;
	float countdown_init = 0.0f;
};

#endif // !__MOUSEEMULATOR_H__



