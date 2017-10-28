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
