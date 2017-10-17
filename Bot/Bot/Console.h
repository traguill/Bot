#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <map>
#include <vector>
#include <string>
#include "Cmd.h"

using namespace std;

class Cmd;
struct CmdUserIn;

class Console
{
public:
	Console();
	~Console();

	void Start();

	bool Execute(const char* cmd)const;

	bool RegisterCommand(Cmd& cmd);


private:
	bool SplitCommand(const char* cmd, CmdUserIn& result)const;

	void LoadDefaultCommands();

	Cmd CreateCommand(const char* name, const char* description, defFunction func);
	void CreateOption(char option, const char* description, defFunction func, Cmd& command);

private:
	map<string, Cmd> commands;
public:
	bool quit_request = false;
};

extern Console* console;

#endif 
