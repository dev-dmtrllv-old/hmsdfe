#include "tasky/TaskScheduler.hpp"

namespace ion::tasky::scheduler
{
	std::vector<Executor*> Executor::executors_;

	bool Executor::stealTask(TaskPtr& task)
	{
		const auto l = executors_.size();
		for (std::size_t i = 0; i < l; i++)
		{
			const auto index = stealIndex_++ % l;
			if (index != exectorIndex_)
			{
				task = executors_.at(index)->queue_.pop();
				if (task != nullptr)
					return true;
			}
		}
		return false;
	}

	bool Executor::runNext()
	{
		TaskPtr ptr = queue_.pop();

		if (ptr == nullptr && !stealTask(ptr))
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