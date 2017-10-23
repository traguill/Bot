#ifndef __BLACKBOARD_H__
#define __BLACKBOARD_H__

#include <string>
#include <map>
#include "Data.h"

using namespace std;

/*
	Accepted types:
	*int
	*float
	*bool
	*string
	*vector
	*Area?
*/

enum BBType
{
	BB_INT = 0,
	BB_FLOAT = 1,
	BB_BOOL = 2,
	BB_STRING = 3,
	BB_VECTOR = 4,
	BB_AREA = 5
};

struct BBVar
{
	string name;
	BBType type;
	char* value;
};

class BlackBoard
{
public:
	BlackBoard();
	~BlackBoard();

	//Load vars
	bool Load(const char* filename);

private:
	void LoadBBInt(Data& data);
	void LoadBBFloat(Data& data);
	void LoadBBString(Data& data);
	void LoadBBBool(Data& data);
	void LoadBBVector(Data& data);
	void LoadBBArea(Data& data);

private:
	map<string, BBVar*> bb_vars;
};
#endif // !__BLACKBOARD_H__

