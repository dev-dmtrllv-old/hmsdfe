#include "async/Worker.hpp"

namespace ion::async
{
	Worker::Worker():
		event_(CreateEvent(NULL, false, false, NULL)),
		isRunning_(true),
		thread_([ & ]() { run(); })
	{ }

	Worker::~Worker()
	{
		stop();
	}

	bool Worker::pop(AsyncWork& work)
	{
		std::unique_lock lk(queueMutex_);
		if (!workQueue_.size())
			return false;
		work = workQueue_.front();
		workQueue_.pop();
		return true;
	}

	void Worker::postWork(AsyncWork&& work)
	{
		{
			std::unique_lock lk(queueMutex_);
			workQueue_.push(work);
		}
		SetEvent(event_);
	}

	void Worker::postWork(AsyncWork::Callback callback, AsyncWork::Callback doneCallback = nullptr, void* data)
	{
		{
			std::unique_lock lk(queueMutex_);
			workQueue_.emplace(callback, doneCallback, data);
		}
		SetEvent(event_);
	}


	void Worker::stop()
	{
		isRunning_.store(false, std::memory_order::release);
		SetEvent(event_);
		if (thread_.joinable())
			thread_.join();
	}

	void Worker::run()
	{
		AsyncWork work;
		while (isRunning_.load(std::memory_order::acquire))
		{
			switch (WaitForSingleObjectEx(event_, INFINITE, true))
			{
				case WAIT_ABANDONED:
					MessageBoxA(NULL, "WAIT_ABANDONED", "WAIT_ABANDONED", MB_OK);
					break;
				case WAIT_IO_COMPLETION:
					MessageBoxA(NULL, "WAIT_IO_COMPLETION", "WAIT_IO_COMPLETION", MB_OK);
					break;
				case WAIT_OBJECT_0:
					while (pop(work))
					{
						work.callback(work.data);
						work.doneCallback(work.data);
					}
					break;
				case WAIT_TIMEOUT:
					MessageBoxA(NULL, "WAIT_TIMEOUT", "WAIT_TIMEOUT", MB_OK);
					break;
				case WAIT_FAILED:
					MessageBoxA(NULL, "WAIT_FAILED", "WAIT_FAILED", MB_OK);
					break;
			}
		}
	}
}