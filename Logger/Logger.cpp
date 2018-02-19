// Clib includes
#include <cstring>

// Middleware include
// None

// Source include
#include "Logger.hpp"
#include "Garbage.hpp"
#include "Assert.hpp"

Logger* Logger::instance = nullptr;

Logger* Logger::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new Logger();
		ASSERT(instance != nullptr);

		instance->Log("Logger created");
	}
	return instance;
}

void Logger::Log(char* pData)
{
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

void Logger::Log(uint8_t* pData, uint8_t len, uint8_t id)
{
	char tab[100];
	int size = sprintf(tab, "Msg from 0x%x", id);
	if(len != 0)
	{
		int size1 = sprintf(&tab[size], ": ");
		size += size1;
	}
	for(size_t i = 0; i < len; i++)
	{
		int size1 = sprintf(&tab[size], " 0x%x", pData[i]);
		size += size1;
	}
	Log(tab);
}

