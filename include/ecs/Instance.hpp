#pragma once

#include "pch.hpp"

namespace ion::ecs
{
	class Arch;

	class Instance
	{
	public:
		Instance();

		inline Arch& rootArch() { return *getArchPtr(0); }
	
		template<typename T>
		inline Arch& getArch() { return *getArchPtr(T::bitmask()); }

		template<typename... T>
		inline Arch& getArch()
		{ 
			std::vector<std::size_t> bitmasks = { (T::bitmask())... };
			std::size_t b = 0;
			for (auto b_ : bitmasks)
				b |= b_;
			return *getArchPtr(b); 
		}

	private:
		void registerArch(std::size_t bitmask);
		Arch* getArchPtr(std::size_t bitmask);

		std::unordered_map<std::size_t, Arch> archMap_;

		friend class Arch;
	};
}