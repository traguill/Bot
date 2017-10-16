#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <map>
#include <vector>
#include <string>

using namespace std;

class Cmd;
struct CmdUserIn;

void Testing(const vector<string>* args);

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

private:
	map<string, Cmd> commands;
};



#endif 
