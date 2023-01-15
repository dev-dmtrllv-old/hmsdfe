#pragma once

#include <vector>
#include <assert.h>

#include "TaskQueue.hpp"
#include "Task.hpp"

namespace ion::tasky
{
	namespace scheduler
	{
		struct Executor
		{
		public:
			static inline Executor create() { return Executor(); }
		private:
			Executor() { printf("Executor() %zu\n", std::this_thread::get_id()); }
		public:
			Executor(const Executor&) = delete;
			Executor(Executor&&) = delete;
			~Executor() noexcept { printf("~Executor() %zu\n", std::this_thread::get_id()); }

		public:
			bool runNext();

			template<typename T>
			void schedule(Task<T> task)
			{
				queue_.push(task.handle().address());
			}

			template<typename Task, typename... Tasks>
			void schedule(Task task, Tasks&&... tasks)
			{
				queue_.push(task.handle().address());
				schedule(std::forward<Tasks>(tasks)...);
			}

		private:
			TaskQueue queue_;
		};

		static Executor& executor()
		{
			static thread_local Executor instance = Executor::create();
			return instance;
		}

		template<typename... Tasks>
		void schedule(Tasks&&... tasks)
		{
			executor().schedule(std::forward<Tasks>(tasks)...);
		}

		template<typename T>
		void buildTasksList(std::vector<TaskPtr>& ptrs, T task)
		{
			ptrs.emplace_back(task.handle().address());
		}

		template<typename T, typename... Tasks>
		void buildTasksList(std::vector<TaskPtr>& ptrs, T task, Tasks... tasks)
		{
			buildTasksList(ptrs, std::forward<T>(task));
			buildTasksList(ptrs, std::forward<Tasks>(tasks)...);
		}

		template<typename T>
		TasksAwaiter await(T task)
		{
			std::vector<TaskPtr> taskPtrs;
			buildTasksList(taskPtrs, std::forward<T>(task));
			return TasksAwaiter(taskPtrs);
		}

		template<typename... T>
		TasksAwaiter await(T... tasks)
		{
			std::vector<TaskPtr> taskPtrs;
			buildTasksList(taskPtrs, std::forward<T>(tasks)...);
			return TasksAwaiter(taskPtrs);
		}

		template<typename T = void>
		TasksAwaiter await(std::vector<Task<T>>& tasks)
		{
			const size_t l = tasks.size();
			std::vector<TaskPtr> taskPtrs(l);
			for (std::size_t i = 0; i < l; i++)
				taskPtrs[i] = tasks.at(i).handle().address();
			return TasksAwaiter(taskPtrs);
		}

		template<typename Container, typename Callback>
		TasksAwaiter awaitEach(Container& container, Callback callback)
		{
			std::vector<TaskPtr> taskPtrs;
			for (auto& item : container)
				buildTasksList(taskPtrs, callback(item));
			return TasksAwaiter(taskPtrs);
		}

		template<typename Callback, typename IndexType = std::size_t>
		TasksAwaiter awaitEach(IndexType count, Callback callback)
		{
			std::vector<TaskPtr> taskPtrs;
			for (IndexType i = 0; i < count; i++)
				buildTasksList(taskPtrs, callback(i));
			return TasksAwaiter(taskPtrs);
		}
	};
}