#include "engine/Engine.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/SceneManager.hpp"

namespace ion::engine
{
	Engine::Engine():
		isInitialized_(false)
	{
		puts("Engine()");
	}

	Engine::~Engine()
	{
		puts("~Engine()");
	}

	void Engine::onInitialize() noexcept
	{
		puts("Engine::onInitialize()");
		auto& rm = SubSystem<ResourceManager>::initialize();
		auto& sm = SubSystem<SceneManager>::initialize();
	}

	void Engine::onTerminate() noexcept
	{
		puts("Engine::onTerminate()");
		SubSystemRegistry::terminate();
	}
}