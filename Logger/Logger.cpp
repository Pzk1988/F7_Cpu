// Clib includes
#include <string.h>

// Middleware include
// None

// Source include
#include "Logger.hpp"
#include "Garbage.hpp"
#include "Assert.hpp"

Logger* Logger::instance = nullptr;

void Logger::Log(char* pData)
{
//	UdpWrite(pData, strlen(pData));
	char tab[203];
	uint8_t len = strlen(pData);
	memcpy(tab, pData, len);
    tab[len] = '\r';
    tab[len + 1] = '\n';
    tab[len + 2] = '\0';

	printf("%s", tab);
}

void Logger::Log(const char* format, ...)
{
	char tab[200];
	va_list args;
	va_start( args, format );
	vsprintf( tab, format, args );
	Log(tab);
	va_end( args );
}

Logger* Logger::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new Logger();
		ASSERT(instance != nullptr);

		instance->Log("Logger created");
	}
	return instance;
};

