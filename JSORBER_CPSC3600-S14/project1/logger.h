#ifndef _LOGGER_H_
#define _LOGGER_H_

#define LOG_PHY 0x01
#define LOG_L1 0x02
#define LOG_L2 0x04
#define LOG_L3 0x08
#define LOG_L4 0x10
#define LOG_CLIENT 0x20

#ifndef LOG_SETTINGS
//default is to log everything
#define LOG_SETTINGS (LOG_PHY | LOG_L1 | LOG_L2 | LOG_L3 | LOG_L4 | LOG_CLIENT)

//log everything from L3 on up.
//#define LOG_SETTINGS (LOG_L3 | LOG_L4 | LOG_CLIENT)
#endif

void output(int priority, const char *format, ...);

#endif //__LOGGER_H_