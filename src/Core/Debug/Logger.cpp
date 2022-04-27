#include "Core/Debug/Logger.h"

Logger& Logger::Get()
{
	static Logger instance;
	return instance;
}

void Logger::AddLog(const Log& log)
{
	m_logs.push_back(log);
}

void Logger::ClearLogs()
{
	m_logs.clear();
}
