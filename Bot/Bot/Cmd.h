#ifndef __CMD_H__
#define __CMD_H__

#include <map>

using namespace std;

struct CmdOption
{
	char option;
	char description[128];
	void(*function)(int argc, char* argv[]);
};

class Cmd
{
public:
	Cmd();
	~Cmd();

private:
	char command[8];
	char description[128];
	map<char, CmdOption> options;
	void (*function)(int argc, char* argv[]);
};
#endif // !__CMD_H__

