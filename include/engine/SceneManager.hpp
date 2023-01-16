#pragma once

#include "engine/SubSystem.hpp"
#include "tasky/task.hpp"
#include "utils/Hash.hpp"

namespace ion::engine
{
	class Scene;

	class SceneManager : public SubSystem<SceneManager>
	{
		using Hash = utils::Hash;
		using Path = std::filesystem::path;
		template<typename T = void>
		using Task = tasky::Task<T>;

	public:
		Task<> loadScene(const Path& path);
		Task<> loadScene(const char* name);
		Task<> loadScene(Hash hash);
		Task<> loadScene(const Scene& scene);

		template<typename T>
		inline Scene& registerScene(const char* name)
		{
			registerScene<T>(ion::utils::hash(name));
		}

		template<typename T>
		inline Scene& registerScene(Hash hash)
		{
			assert(!scenes_.contains(hash));
			scenes_.emplace(std::piecewise_construct, std::forward_as_tuple(hash), std::forward_as_tuple(new T()));
		}

		inline Scene& registerScene(const Path& path)
		{
			throw std::runtime_error("TODO: inline Scene& registerScene(const Path& path)!");
		}

	protected:
		void onInitialize() override {}
		void onTerminate() override;
	
	private:
		std::unordered_map<Hash, Scene*> scenes_;
	};
}