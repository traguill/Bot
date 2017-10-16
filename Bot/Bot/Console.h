#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <map>

using namespace std;

class Cmd;
struct CmdUserIn;

class Console
{
public:
	Console();
	~Console();

	bool Execute(const char* cmd)const;

	bool RegisterCommand();

private:
	bool SplitCommand(const char* cmd, CmdUserIn& result)const;

private:
	map<const char*, Cmd> commands;
};

#endif 
