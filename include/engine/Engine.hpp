#pragma once

#include "pch.hpp"
#include "engine/MainWorker.hpp"

namespace ion::engine
{
	class Game;

	class Engine
	{
	private:
		static inline Engine& getMutable() noexcept
		{
			static Engine instance_;
			return instance_;
		}
	public:
		static inline const Engine& get() noexcept
		{
			static Engine instance_;
			return instance_;
		}
		
		template<typename Game>
		static inline const Engine& initialize() noexcept
		{
			auto& engine = getMutable();
			engine.onInitialize(new Game());
			engine.isInitialized_ = true;
			return engine;
		}

		static inline void terminate() noexcept
		{
			auto& engine = getMutable();
			assert(engine.isInitialized_);
			engine.onTerminate();
			engine.isInitialized_ = false;
		}

	private:
		Engine();
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		~Engine();

		void onInitialize(Game* game) noexcept;
		void onTerminate() noexcept;

	public:
		void run() const;
		inline Game& game() const noexcept { assert(game_ != nullptr); return *game_; }
		inline bool isRunning() const noexcept { return isRunning_.load(std::memory_order::acquire); }

	private:
		MainWorker worker_;
		Game* game_;
		bool isInitialized_;
		mutable std::atomic<bool> isRunning_;
	};
}