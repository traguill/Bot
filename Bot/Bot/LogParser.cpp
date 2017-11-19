#include "LogParser.h"
#include <stdio.h>
#include "ConsoleMsgs.h"

LogParser::LogParser()
{
}

LogParser::~LogParser()
{
}

void LogParser::Update() //Doesn't work
{
	log_path = "C:\\Program Files (x86)\\Hearthstone\\Logs\\Power.log";
	FILE* p_file = fopen(log_path.data(), "rb");
	if (p_file == NULL)
	{
		MSG_ERROR("Error openng the file %s", log_path);
		return;
	}

	//Obtain the size:
	fseek(p_file, 0, SEEK_END);
	long f_size = ftell(p_file);

	long new_size = f_size - file_pointer;
	char* buf = new char[new_size*sizeof(char)];
	std::string buf_s;
	buf_s.reserve(new_size);

	fseek(p_file, file_pointer, SEEK_SET);
	size_t ret_size = fread(&buf[0], 1, new_size, p_file);
	if (ret_size != new_size)
	{
		MSG_ERROR("Error reading the file %s", log_path);
	}
	else
	{
		buf_s = buf;
		//Print line by line
		std::string end_line = "\r\n";
		size_t sb_pointer = 0;
		size_t se_pointer = 0;

		while (se_pointer != std::string::npos)
		{
			se_pointer = buf_s.find(end_line, sb_pointer);
			if (se_pointer != std::string::npos)
			{
				std::string line = buf_s.substr(sb_pointer, se_pointer - sb_pointer);
				MSG_INFO("%s", line.data());
				sb_pointer = se_pointer+1;
			}
		}
	}
	file_pointer = f_size;
	if (buf)
		delete[] buf;
	fclose(p_file);
}

