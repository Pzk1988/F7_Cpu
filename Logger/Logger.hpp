#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <stdarg.h>

class Logger
{
public:
	virtual ~Logger() = default;
	static Logger* GetInstance();
	void Log(char* pData, uint8_t len);
	void LogV(const char* format, ...);

private:
	static Logger* instance;
	Logger() = default;
};

#endif // LOGGER_H
