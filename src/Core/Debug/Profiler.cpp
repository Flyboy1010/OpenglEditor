#include "Core/Debug/Profiler.h"

/* PROFILER */

Profiler& Profiler::Get()
{
	static Profiler instance;
	return instance;
}

void Profiler::AddProfile(const Profile& timing)
{
	m_profiles.push_back(timing);
}

void Profiler::ClearProfiles()
{
	m_profiles.clear();
}

/* TIMER */

Timer::Timer(const std::string& name)
{
	m_name = name;
	m_startTimePoint = std::chrono::high_resolution_clock::now();
	m_duration = 0.0f;
}

Timer::~Timer()
{
	// stop the timer

	Stop();

	// add a profile to the profiler

	Profiler::Get().AddProfile({ m_name, m_duration });
}

void Timer::Stop()
{
	auto endTimePoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	m_duration = (end - start) * 0.001f;
}
