#pragma once

#include "pch.hpp"

namespace ion::async
{
	class Worker
	{
	public:
		struct AsyncWork
		{
			using Callback = void(*)(void* data);
			
			AsyncWork(Callback callback = nullptr, Callback doneCallback = nullptr, void* data = nullptr):
				callback(callback),
				doneCallback(doneCallback),
				data(data)
			{}

			Callback callback;
			Callback doneCallback;
			void* data;
		};

		static inline Worker& get()
		{
			static Worker worker;
			return worker;
		}

	private:
		Worker();
		Worker(const Worker&) = delete;
		Worker(Worker&&) = delete;
		~Worker();

		void run();
		

	public:
		bool pop(AsyncWork& work);
		void postWork(AsyncWork&& work);
		void postWork(AsyncWork::Callback callback, AsyncWork::Callback doneCallback, void* data = nullptr);

		void stop();

	private:
		HANDLE event_;
		std::atomic<bool> isRunning_;
		std::thread thread_;
		std::mutex queueMutex_;
		std::queue<AsyncWork> workQueue_;
	};
}