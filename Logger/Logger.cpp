#include <Logger.hpp>
#include "Garbage.hpp"

Logger* Logger::instance = nullptr;

void Logger::Log(char* pData, uint8_t len)
{
	UdpWrite(pData, len);
}

void Logger::LogV(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

Logger* Logger::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new Logger();
	}
	return instance;
};

