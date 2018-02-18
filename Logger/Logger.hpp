#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <stdarg.h>

class Logger
{
public:
	virtual ~Logger() = default;
	static Logger* GetInstance();
	void Log(char* pData);
	void Log(const char* format, ...);

private:
	static Logger* instance;
	Logger() = default;
};

#endif // LOGGER_H
