#pragma once

#include "pch.hpp"

namespace ion::ecs
{
	class Instance;
	class Arch;

	struct ComponentInfo
	{
		std::size_t size;
		std::size_t index;
		std::size_t bitmask;
		
		ComponentInfo(std::size_t size, std::size_t index, std::size_t bitmask):
			size(size),
			index(index),
			bitmask(bitmask)
		{ }
	};

	struct ComponentRegistery
	{
	private:
		static inline std::vector<ComponentInfo> components_ = {};
		static inline std::size_t indexCounter_ = 0;

		template<typename T>
		friend void registerComponentType();

		friend class Instance;
		friend class Arch;
	};

	template<typename T>
	struct Component
	{
	public:
		Component() { }

		static std::size_t index_;
		static std::size_t bitmask_;
		static bool isRegistered_;

	protected:
		friend class ArchManager;
		friend class Arch;
		friend class GameObject;

	public:
		static std::size_t index() noexcept { return index_; };
		static std::size_t bitmask() noexcept { return bitmask_; };
		static bool isRegistered() noexcept { return isRegistered_; };

		template<typename T>
		friend void registerComponentType();
	};

	template <class... T>
	std::vector<std::size_t> getComponentsBitmasks()
	{
		return { (T::bitmask())... };
	}

	template<typename T>
	void registerComponentType()
	{
		assert(!T::isRegistered());
		T::index_ = ComponentRegistery::indexCounter_++;
		T::bitmask_ = 1ULL << T::index_;
		T::isRegistered_ = true;
		const char* type = typeid(T).name();
		ComponentRegistery::components_.emplace_back(sizeof(T), T::index_, T::bitmask_);
	}

	template<typename T>
	std::size_t Component<T>::index_ = 0;

	template<typename T>
	std::size_t Component<T>::bitmask_ = 0;

	template<typename T>
	bool Component<T>::isRegistered_ = false;

	template<typename T>
	concept ExtendsComponent = std::is_base_of_v<Component<T>, T>;
}