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
	void Log(uint8_t* pData, uint8_t len, uint8_t id);
	void Log(uint8_t* pData, uint32_t extId, uint8_t len, uint8_t id);

private:
	static Logger* instance;
	Logger() = default;
};

#endif // LOGGER_H
