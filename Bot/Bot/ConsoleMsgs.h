#ifndef __CONSOLEMSG_H__
#define __CONSOLEMSG_H__

#define MSG_ERROR PrintErrorMessage
#define MSG_INFO PrintInfoMessage
#define MSG_WARNING PrintWarningMessage

//TODO: Change the color of each msg
void PrintErrorMessage(const char* format, ...);
void PrintInfoMessage(const char* format, ...);
void PrintWarningMessage(const char* format, ...);

#endif // 
