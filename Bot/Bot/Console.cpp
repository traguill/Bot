#include "Console.h"
#include "Application.h"

#include "Commands.h"
#include "ConsoleHeader.h"

Console::Console()
{}

Console::~Console()
{}

void Console::Start()
{
	LoadDefaultCommands();
}

void Console::PrintHeader() const
{
	if (header_enabled && current_header)
	{
		current_header->PrintHeader();
	}
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

bool Console::RequestHeader(ConsoleHeader * hc)
{
	if (hc)
	{
		header_enabled = true;
		current_header = hc;
	}
	else
	{
		current_header = nullptr;
		header_enabled = false;
	}
	return true; //TODO: Improve this approach
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
				printf("Invalid command (%s). Name is too long", &cmd_name[0]);
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
	//exit
	Cmd exit = CreateCommand("exit", "Quits the application", (defFunction)(&Quit));
	RegisterCommand(exit);

	//clear
	Cmd clear = CreateCommand("clear", "Clears the console.", (defFunction)(&ClearConsoleScreen));
	RegisterCommand(clear);

	//area
	Cmd area = CreateCommand("area", "Handles the areas where the bot will interact.", NULL);

	CreateOption('c', "Creates a new area.", CreateArea, area); // -c create
	CreateOption('l', "Lists all areas.", ListAreas, area); // -l list
	CreateOption('r', "<area_to_remove> Removes area_to_remove from the list", RemoveArea, area); // -r remove
	CreateOption('e', "Edit an area.\n Use 'area -e rename <area_to_rename> <new_name>' to change the name.\n Use 'area -e space <area>' to edit it.", EditArea, area);
	CreateOption('s', "Makes an area visible.", ShowArea, area); //-s show

	RegisterCommand(area);

	//move
	Cmd move = CreateCommand("move", "Moves the mouse and handles clicks", (defFunction)(&MoveMouseToArea));
	RegisterCommand(move);

	//bb
	Cmd bb = CreateCommand("bb", "Blackboard", nullptr);

	CreateOption('s', "Shows all the variables inside the BlackBoard and its values. Usage: bb -s <name>", BBShow, bb);
	CreateOption('i', "Inserts a variable inside the Blackboard. Usage: -i <type> <name> <value>.bool/int/float/string/vector/area.", BBInsert, bb);

	RegisterCommand(bb);

	//bt
	Cmd bt = CreateCommand("bt", "BehaviorTree", nullptr);

	CreateOption('n', "Creates a new BT. Usage: bt -n <bt_name>", BTNew, bt);
	CreateOption('s', "Shows the BTs available", ShowBTs, bt);
	CreateOption('e', "Sets a BT to edit it. Usage: bt -e <name>", BTEdit, bt);
	CreateOption('i', "Inserts a new node. Usage: bt -i <type> <sub-type>", BTInsert, bt);

	RegisterCommand(bt);
}

Cmd Console::CreateCommand(const char * name, const char * description, defFunction func) 
{
	Cmd c;

	strcpy_s(&c.command[0], sizeof(char) * 8, name);
	strcpy_s(&c.description[0], sizeof(char) * 128, description);
	
	if (func != NULL)
		c.f = func;

	return c;
}

void Console::CreateOption(char option, const char * description, defFunction func, Cmd & command)
{
	CmdOption o;

	o.option = option;
	strcpy_s(&o.description[0], sizeof(char) * 128, description);
	o.f = func;

	command.options.insert(pair<char, CmdOption>(o.option, o));
}
