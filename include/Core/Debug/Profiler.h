#pragma once

#include <vector>
#include <string>
#include <chrono>

#define PROFILE(name) Timer t(name);

struct Profile
{
	std::string name;
	float duration;
};

class Profiler
{
public:
	static Profiler& Get();

	const std::vector<Profile>& GetProfiles() const { return m_profiles; }

	void AddProfile(const Profile& timing);

	void ClearProfiles();

private:
	Profiler() {}
	~Profiler() {}

private:
	std::vector<Profile> m_profiles;
};

class Timer
{
public:
	Timer(const std::string& name);
	~Timer();

	void Stop();

private:
	std::string m_name;
	std::chrono::time_point<std::chrono::steady_clock> m_startTimePoint;
	float m_duration;
};