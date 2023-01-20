#pragma once

#include "tasky/TaskScheduler.hpp"

namespace ion::engine
{
	class Engine;

	class MainWorker
	{
		using Executor = tasky::scheduler::Executor;

	public:
		MainWorker(const Engine& engine);
		MainWorker(const MainWorker&) = delete;
		MainWorker(MainWorker&&) = delete;
		~MainWorker();

		void enterEventLoop() const;

		const Engine& engine;

	private:
		Executor& executor_;
	};
}