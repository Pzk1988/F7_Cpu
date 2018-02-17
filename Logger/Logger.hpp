#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger
{
public:
	virtual ~Logger() = default;
	static Logger* GetInstance();
	void Log(std::string);

private:
	static Logger* instance;
	Logger() = default;
};

#endif // LOGGER_H
