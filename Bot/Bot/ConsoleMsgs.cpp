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

	SetConsoleTextAttribute(App->h_console, 0x07); //Reset to white
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
	SetConsoleTextAttribute(App->h_console, 0x06); //Yellow
	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);

	SetConsoleTextAttribute(App->h_console, 0x07); //Reset to white
}