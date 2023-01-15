#include "tasky/TaskScheduler.hpp"

namespace ion::tasky::scheduler
{
	bool stealNext(TaskPtr& ptr)
	{
		// TODO: steal from other queue :D
		return false;
	}

	bool Executor::runNext()
	{
		TaskPtr ptr = queue_.pop();

		if(ptr == nullptr && !stealNext(ptr))
			return false;
		
		if (ptr != nullptr)
		{
			auto handle = std::coroutine_handle<TaskPromiseBase>::from_address(ptr);
			auto& promise = handle.promise();

			if (promise.resume())
			{
				if (promise.tasksAwaiter_) [[likely]]
				{
					const std::size_t count = promise.tasksAwaiter_->tasks_.size() - 1;
					if (count > 0)
						queue_.addTaskCount(count);
					for (TaskPtr ptr : promise.tasksAwaiter_->tasks_)
					{
						auto h = std::coroutine_handle<TaskPromiseBase>::from_address(ptr);
						h.promise().parentCoroutine_ = handle;
						queue_.push(ptr);
					}
				}
				else [[unlikely]]
				{
					throw std::runtime_error("A coroutine got enqueued while resuming! The coroutine should or return, yield or await!");
				}
			}
			else if (promise.exception_.has_value())
			{
				std::rethrow_exception(promise.exception_.value());
			}
			else if (promise.parentCoroutine_)
			{
				auto& parentPromise = promise.parentCoroutine_.promise();
				if (parentPromise.tasksAwaiter_)
				{
					if (parentPromise.tasksAwaiter_->doneCount_.fetch_sub(1, std::memory_order::acq_rel) - 1 == 0)
					{
						queue_.push(promise.parentCoroutine_.address());
					}
					else
					{
						queue_.removeTaskCount(1);
					}
				}
				else
				{
					queue_.push(promise.parentCoroutine_.address());
					handle.destroy();
				}
			}
			else
			{
				queue_.removeTaskCount(1);
				handle.destroy();
			}
		}
		
		return true;
	}
}