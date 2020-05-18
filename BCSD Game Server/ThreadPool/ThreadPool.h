#pragma once

#include "stdafx.h"

class ThreadPool
{
public:
	using Function = std::function<void()>;

	static ThreadPool* GetInstance() noexcept;

	DWORD		GetThreadCount() const noexcept;
	bool		Add(Function job) noexcept;

private:
	ThreadPool() noexcept;
	~ThreadPool() noexcept;
	void	work() noexcept;

	bool						isAllStop			= false;
	DWORD						threadCount;
	SRWLOCK						srw					= SRWLOCK_INIT;
	CONDITION_VARIABLE			cv;
	std::vector<std::thread>	workerThreads;
	std::queue<Function>		jobQueue;
};