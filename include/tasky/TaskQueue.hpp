#pragma once

#include <queue>
#include <mutex>
#include <deque>

namespace ion::tasky
{
	class TaskQueue
	{
	public:
		TaskQueue() {  }
		~TaskQueue() {  }

		[[nodiscard]] inline bool isDone() const noexcept
		{
			return tasksCount_.load(std::memory_order::acquire) == 0;
		}

		inline void addTaskCount(std::size_t count = 1) noexcept { tasksCount_.fetch_add(count, std::memory_order::acq_rel); };
		inline void removeTaskCount(std::size_t count = 1) noexcept { tasksCount_.fetch_sub(count, std::memory_order::acq_rel); };

		inline void push(void* task)
		{
			std::unique_lock lk(mutex_);
			tasks_.emplace_back(task);
		}

		[[nodiscard]] inline void* pop()
		{
			std::unique_lock lk(mutex_);
			
			if(tasks_.size() == 0)
				return nullptr;

			void* ptr = tasks_.back();
			tasks_.pop_back();
			return ptr;
		}

		[[nodiscard]] inline void* steal()
		{
			std::unique_lock lk(mutex_);
			
			if(tasks_.size() == 0)
				return nullptr;

			void* ptr = tasks_.front();
			tasks_.pop_front();
			return ptr;
		}

	private:
		std::atomic<std::size_t> tasksCount_;
		std::deque<void*> tasks_;
		std::mutex mutex_;
	};
}