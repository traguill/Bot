#ifndef __LOGPARSER_H__
#define __LOGPARSER_H__

#include <string>


class LogParser
{
public:
	LogParser();
	~LogParser();

	void Update();

private:

	std::string log_path;
	long file_pointer = 0;
};
#endif // !__LOGPARSER_H__
