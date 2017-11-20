#ifndef __CONSOLEMSG_H__
#define __CONSOLEMSG_H__

//Colors: http://1.bp.blogspot.com/-fnyGzmRKSng/UDGuyMOXoBI/AAAAAAAAACs/cDbRvmu1LTE/s1600/ConsoleColor.bmp

#define MSG_ERROR PrintErrorMessage
#define MSG_INFO PrintInfoMessage
#define MSG_WARNING PrintWarningMessage
#define BT_HEADER PrintConsoleHeader
#define MSG_WIN_LASTERROR PrintWindowsLastError
#define HS_LOG PrintHSLogMessage

void PrintErrorMessage(const char* format, ...);
void PrintInfoMessage(const char* format, ...);
void PrintWarningMessage(const char* format, ...);
void PrintWindowsLastError();
void PrintConsoleHeader(const char* bt_name, const char* format, ...);
void PrintHSLogMessage(const char* format, ...);


#endif // 
