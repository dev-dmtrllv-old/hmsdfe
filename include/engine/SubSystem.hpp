#pragma once

#include "pch.hpp"
#include <functional>

namespace ion::engine
{
	
	namespace SubSystemRegistry
	{
		void terminate();
	}

	class SubSystemBase
	{
	protected:
		static inline std::stack<std::function<void()>> terminateCallbacks_ = {};

		SubSystemBase() { }
		SubSystemBase(const SubSystemBase&) = delete;
		SubSystemBase(SubSystemBase&&) = delete;
		virtual ~SubSystemBase() { }

		friend void SubSystemRegistry::terminate();
	};

	
	namespace SubSystemRegistry
	{
		void terminate()
		{
			while(SubSystemBase::terminateCallbacks_.size() > 0)
			{
				auto& terminator = SubSystemBase::terminateCallbacks_.top();
				terminator();
				SubSystemBase::terminateCallbacks_.pop();
			}	
		}
	}

	template<typename T>
	class SubSystem: public SubSystemBase
	{
	private:
		static inline SubSystem<T>* instance_ = nullptr;

	public:
		static inline T& get()
		{
			if(T::instance_ == nullptr)
				T::instance_ = new T();
			return *static_cast<T*>(T::instance_);
		}

		static inline T& initialize()
		{	
			auto& instance = get();
			assert(!instance.isInitialized_);
			
			const char* name = typeid(T).name();
			printf("SubSystem<%s>::initialize()\n", name);

			T::instance_->onInitialize();
			terminateCallbacks_.emplace([](){ terminate(); });
			return instance;
		}

		static inline void terminate()
		{
			assert(T::instance_ != nullptr);
			
			const char* name = typeid(T).name();
			printf("SubSystem<%s>::terminate()\n", name);
			
			T::instance_->onTerminate();
			T::instance_ = nullptr;
		}

	protected:
		SubSystem(): SubSystemBase() { }
		SubSystem(const SubSystem&) = delete;
		SubSystem(SubSystem&&) = delete;
		virtual ~SubSystem() { }

	public:
		virtual void onInitialize() = 0;
		virtual void onTerminate() = 0;

	private:
		bool isInitialized_ = false;
	};
}