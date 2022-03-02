#pragma once

#include <thread>
#include <condition_variable>
#include <functional>
#include <queue>
#include <mutex>

class ThreadPool
{
public:
	ThreadPool(int threadsCount = std::thread::hardware_concurrency());
	~ThreadPool();

	void PushTask(const std::function<void()>& task);

private:
	void DoWork();

private:
	int m_workersCount;
	bool m_working;
	std::vector<std::thread> m_workers;
	std::queue<std::function<void()>> m_tasks;
	std::mutex m_tasksMutex;
	std::condition_variable m_condition;
};