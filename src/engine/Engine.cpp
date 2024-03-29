#include "engine/Engine.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/SceneManager.hpp"
#include "engine/Game.hpp"
#include "tasky/TaskScheduler.hpp"

namespace ion::engine
{
	Engine::Engine():
		worker_(MainWorker(*this)),
		isInitialized_(false)
	{
		puts("Engine()");
	}

	Engine::~Engine()
	{
		puts("~Engine()");
	}

	void Engine::onInitialize(Game* game) noexcept
	{
		puts("Engine::onInitialize()");

		assert(game_ == nullptr && game != nullptr);
		game_ = game;

		[[maybe_unused]] auto& rm = SubSystem<ResourceManager>::initialize();
		[[maybe_unused]] auto& sm = SubSystem<SceneManager>::initialize();
	}

	void Engine::onTerminate() noexcept
	{
		assert(game_ != nullptr);
		delete game_;
		puts("Engine::onTerminate()");
		SubSystemRegistry::terminate();
	}

	void Engine::run() const
	{
		game().onLoad();
		isRunning_ = true;
		worker_.enterEventLoop();
	}
}