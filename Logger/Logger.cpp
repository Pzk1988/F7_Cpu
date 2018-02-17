#include <Logger.hpp>

Logger* Logger::instance = nullptr;

void Logger::Log(std::string)
{

}

Logger* Logger::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new Logger();
	}
	return instance;
};

