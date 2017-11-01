#include "ConsoleMsgs.h"
#include <stdio.h>      
#include <stdarg.h>
#include <windows.h>
#include "Application.h"

void  PrintErrorMessage(const char* format, ...)
{
	SetConsoleTextAttribute(App->h_console, 0x0C); //Red

	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);

	SetConsoleTextAttribute(App->h_console, 0x0F); //Reset to white
}

void  PrintInfoMessage(const char* format, ...)
{
	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);
}

void  PrintWarningMessage(const char* format, ...)
{
	SetConsoleTextAttribute(App->h_console, 0x0E); //Yellow
	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);

	SetConsoleTextAttribute(App->h_console, 0x0F); //Reset to white
}

void PrintWindowsLastError()
{
	char* buf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buf, 0, NULL);
	if (buf)
	{
		MSG_ERROR("WinLastError: %s", buf);
		delete[] buf;
	}
}

void PrintConsoleHeader(const char * bt_name, const char * format, ...)
{
	SetConsoleTextAttribute(App->h_console, 0x0A); //Green
	printf("$ BT:%s", bt_name);
	
	SetConsoleTextAttribute(App->h_console, 0x0E); //Yellow
	printf("~");
	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);
	SetConsoleTextAttribute(App->h_console, 0x0F); //Reset to white
}
