#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <map>

using namespace std;

class Cmd;

class Console
{
public:
	Console();
	~Console();

	bool Execute(const char* cmd)const;

	bool RegisterCommand();

private:
	map<const char*, Cmd> commands;
};

#endif 
