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

	void Init(const char* filename, const char* name = nullptr);

	void PrintVars()const;

	//Insert
	bool InsertBool(const string& name, bool value);
	bool InsertInt(const string& name, int value);
	bool InsertFloat(const string& name, float value);
	bool InsertString(const string& name, const string& value);
	bool InsertArea(const string& name, const string& value);

private:
	//Load vars
	bool Save()const;
	bool Load();

	void LoadBBInt(Data& data);
	void LoadBBFloat(Data& data);
	void LoadBBString(Data& data);
	void LoadBBBool(Data& data);
	void LoadBBVector(Data& data);
	void LoadBBArea(Data& data);

	void SaveBBInt(Data& data, const BBVar* var)const;
	void SaveBBFloat(Data& data, const BBVar* var)const;
	void SaveBBString(Data& data, const BBVar* var)const;
	void SaveBBBool(Data& data, const BBVar* var)const;
	void SaveBBVector(Data& data, const BBVar* var)const;
	void SaveBBArea(Data& data, const BBVar* var)const;

private:
	map<string, BBVar*> bb_vars;
	string filename;
};
#endif // !__BLACKBOARD_H__

