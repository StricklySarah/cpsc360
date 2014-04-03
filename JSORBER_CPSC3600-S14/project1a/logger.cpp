#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

#define LOG_PRE "   "

const char *get_prefix(int log_level)
{
	switch (log_level)
	{
		case LOG_PHY:
			return LOG_PRE LOG_PRE LOG_PRE LOG_PRE LOG_PRE"PHY";
		case LOG_L1:
			return LOG_PRE LOG_PRE LOG_PRE LOG_PRE "L1";
		case LOG_L2:
			return LOG_PRE LOG_PRE LOG_PRE "L2";
		case LOG_L3:
			return LOG_PRE LOG_PRE "L3";
		case LOG_L4:
			return LOG_PRE "L4";
		case LOG_L4B:
			return LOG_PRE "L4B";
		case LOG_CLIENT:
			return "CLIENT";
	}
	return "UNKNOWN";
}

void output(int priority, const char *format, ...)
{
    va_list args;
    va_start(args, format);
	
    if(priority & LOG_SETTINGS)
	{
		printf("%s: ",get_prefix(priority));
    	vprintf(format, args);
		printf("\n");
	}
    va_end(args);
}