#include "engine/SceneManager.hpp"
#include "tasky/task.hpp"
#include "utils/Hash.hpp"
#include "engine/Scene.hpp"

namespace ion::engine
{
	using namespace tasky;

	void SceneManager::onTerminate()
	{
		for (auto& [_, scene] : scenes_)
			delete scene;
		scenes_.clear();
	}

	Task<> SceneManager::loadScene(const Path& path)
	{
		throw std::runtime_error("TODO: Task<> SceneManager::loadScene(const Path& path)!");
	}

	Task<> SceneManager::loadScene(const char* name)
	{
		co_await loadScene(utils::hash(name));
	}

	Task<> SceneManager::loadScene(Hash hash)
	{
		co_return;
	}

	Task<> SceneManager::loadScene(const Scene& scene)
	{
		co_return;
	}

}