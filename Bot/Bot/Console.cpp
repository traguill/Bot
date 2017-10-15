#include "Console.h"
#include "Cmd.h"

Console::Console()
{
}

Console::~Console()
{
}

bool Console::Execute(const char* cmd) const
{
	if (cmd == nullptr)
		return false;

	bool ret = false;

	//Split the command
	char c = 'c';
	int i = 0;
	bool cmd_done = false;
	bool has_options = false;
	bool has_args = false;

	//Command
	char cmd_name[8];
	char option = '\0';
	char** argv;
	int argc = 0;

	while (c != '\0')
	{
		c = cmd[i];

		//Get command name
		if (!cmd_done)
		{
			//Command must be smaller than 8 chars
			if (i >= 8)
			{
				printf("Invalid command (%s). Name is too long", cmd_name);
				return false; //Command name too long
			}

			if (c != ' ') //Writte each cmd character
			{
				cmd_name[i] = c;
				if (c == '\0')
				{
					cmd_done = true; //Only a command name was specified
				}
			}
			else //Space is found. End of the command
			{
				cmd_name[i] = '\0';
				cmd_done = true;

				//Find if next parameter is an option (-) or an argument
				if (cmd[i + 1] == '-')
				{
					has_options = true;
					++i; //Skip the dash
					option = cmd[++i];

					//space or end
					c = cmd[++i];
					if (c == '0')
						break; //only options no arguments
				}
			}
		}
		else //Arguments
		{ //TODO: Replace arguments for a vector of strings
			has_args = true;
			size_t length = strlen(cmd) - i;
			argv = new char*;
			*argv = (char*)malloc(length);
			char a = 'a';
			int i2 = 0;
			argc = 1;
			while (a != '\0')
			{
				a = cmd[i+i2];

				if (a == ' ')
				{
					*(argv[0]+i2) = '\0';
					++argc;
				}
				else
					*(argv[0]+i2) = a;

				++i2;
			}
			break;
		}
		++i;
	}

	printf("Command %s\n", cmd_name);
	if (has_options)
		printf("Option: %c\n", option);

	if (has_args)
	{
		for (int j = 0; j < argc; ++j)
			printf("Arg %d : %s\n", j, argv[j]);
	}

	return ret;
}

bool Console::RegisterCommand()
{
	return false;
}
