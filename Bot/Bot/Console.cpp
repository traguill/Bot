#include "Console.h"
#include "Cmd.h"
#include "Application.h"

Console::Console()
{}

Console::~Console()
{}

void Console::Start()
{
	LoadDefaultCommands();
}

bool Console::Execute(const char* cmd) const
{
	if (cmd == nullptr)
		return true;

	bool ret = false;

	CmdUserIn user_input;
	ret = SplitCommand(cmd, user_input);

	//Debug-----------------------------------------------
	/*printf("Command %s\n", &user_input.command[0]);
	if (user_input.option != NULL)
		printf("Option: %c\n", user_input.option);
	if (user_input.args.size() > 0)
	{
		int count = 0;
		for (auto arg : user_input.args)
		{
			++count;
			printf("Arg(%i): %s\n", count, arg.data());
		}
	}*/

	map<string, Cmd>::const_iterator result = commands.find(user_input.command);

	if (result != commands.end())
	{
		if (result->second.f != NULL) //Only command + args(optional)
		{
			(*(result->second.f))(&user_input.args);
		}
		else
		{
			map<char, CmdOption>::const_iterator cmd_option = result->second.options.find(user_input.option);
			if (cmd_option != result->second.options.end())
			{
				(*(cmd_option->second.f))(&user_input.args);
			}
			else
			{
				printf("Command %s does not accept %c\n", &user_input.command[0], user_input.option);
			}
		}
	}
	else
	{
		printf("%s is not a valid command\n", &user_input.command[0]);
	}

	return ret;
}

bool Console::RegisterCommand(Cmd& cmd)
{
	if (commands.end() == commands.find(cmd.command))
	{
		commands.insert(pair<string, Cmd>(&cmd.command[0], cmd));
		return true;
	}
	return false;
}

bool Console::SplitCommand(const char * cmd, CmdUserIn& result) const
{
	//Split the command
	char c = 'c';
	int i = 0;
	bool cmd_done = false;
	bool has_options = false;
	bool has_args = false;

	//Command
	char cmd_name[8];
	char option = '\0';
	vector<string> args;

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
			string args_s = string(cmd, i, length);

			char* tmp = nullptr;
			tmp = strtok(&args_s[0], " ");
			while (tmp != NULL)
			{
				args.push_back(tmp);
				tmp = strtok(NULL, " ");
			}
			break;
		}
		++i;
	}

	memcpy(&result.command, &cmd_name, sizeof(char) * 8);
	if(has_options)
		result.option = option;

	if (has_args)
	{
		result.args = args;
	}

	return true;
}

void Console::LoadDefaultCommands()
{
	//Exit
	Cmd exit;

	strcpy_s(&exit.command[0], sizeof(char) * 8, "exit");
	const char* desc = "Quits the application";
	strcpy_s(&exit.description[0], sizeof(char) * 128, desc);
	exit.f = &Quit;

	RegisterCommand(exit);
}
