#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

#define LOG(type, format, ...)\
	{\
		int size = snprintf(nullptr, 0, format, __VA_ARGS__) + 1;\
		char* msg = new char[size];\
		snprintf(msg, size, format, __VA_ARGS__);\
		Logger::Get().AddLog({ type, msg });\
	}

#define LOG_INFO(format, ...) LOG(INFO, format, __VA_ARGS__);
#define LOG_WARNING(format, ...) LOG(WARNING, format, __VA_ARGS__);
#define LOG_ERROR(format, ...) LOG(ERROR, format, __VA_ARGS__);

enum LogType
{
	INFO,
	WARNING,
	ERROR
};

struct Log
{
	LogType type;
	std::string msg;
};

class Logger
{
public:
	static Logger& Get();

	const std::vector<Log>& GetLogs() const { return m_logs; }

	void AddLog(const Log& log);

	void ClearLogs();

private:
	Logger() {}
	~Logger() {}
	
private:
	std::vector<Log> m_logs;
};