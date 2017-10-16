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

typedef void(*function)(const vector<string>* args);

struct CmdOption
{
	char option;
	char description[128];
	function f;
};



class Cmd
{
public:
	Cmd();
	~Cmd();

public:
	char command[8];
	char description[128];
	map<char, CmdOption> options;
	function f;
};
#endif // !__CMD_H__

