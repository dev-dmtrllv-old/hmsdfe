#pragma once

#include "pch.hpp"

namespace ion::engine
{
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
		
		static inline const Engine& initialize() noexcept
		{
			auto& engine = getMutable();
			engine.onInitialize();
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

		void onInitialize() noexcept;
		void onTerminate() noexcept;


	private:
		bool isInitialized_;
	};
}