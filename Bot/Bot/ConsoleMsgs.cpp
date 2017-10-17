#include "ConsoleMsgs.h"
#include <stdio.h>      
#include <stdarg.h>

void  PrintErrorMessage(const char* format, ...)
{
	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);
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
	static va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	printf("\n");
	va_end(ap);
}