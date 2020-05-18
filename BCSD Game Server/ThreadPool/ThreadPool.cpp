#include "stdafx.h"
#include "ThreadPool.h"

ThreadPool* ThreadPool::GetInstance() noexcept
{
    static ThreadPool instance;
    return &instance;
}

DWORD ThreadPool::GetThreadCount() const noexcept
{
    return threadCount;
}

bool ThreadPool::Add(Function job) noexcept
{
	if (isAllStop)
	{
		return false;
	}

	AcquireSRWLockExclusive(&srw);
	jobQueue.push(std::move(job));
	WakeConditionVariable(&cv);
	ReleaseSRWLockExclusive(&srw);

	return true;
}

ThreadPool::ThreadPool() noexcept
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	threadCount = info.dwNumberOfProcessors - 1;

	workerThreads.reserve(threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		workerThreads.emplace_back([this]() { this->work(); });
	}
}

ThreadPool::~ThreadPool() noexcept
{
	isAllStop = true;

	WakeAllConditionVariable(&cv);
	for (auto& t : workerThreads)
	{
		t.join();
	}
}

void ThreadPool::work() noexcept
{
	while (true)
	{
		SleepConditionVariableSRW(&cv, &srw, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
		AcquireSRWLockShared(&srw);


		ReleaseSRWLockShared(&srw);
	}
}
