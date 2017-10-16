#ifndef __CMD_H__
#define __CMD_H__

#include <map>
#include <vector>
#include <string>

using namespace std;

struct CmdUserIn
{
	char command[8];
	char option = NULL;
	vector<string> args;
};

struct CmdOption
{
	char option;
	char description[128];
	void(*function)(const vector<string>* args);
};

typedef void(*functions)(const vector<string>* args);

class Cmd
{
public:
	Cmd();
	~Cmd();

public:
	char command[8];
	char description[128];
	map<char, CmdOption> options;
	functions f;
};
#endif // !__CMD_H__

