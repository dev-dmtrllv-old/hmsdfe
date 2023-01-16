#include "ecs/Instance.hpp"
#include "ecs/Arch.hpp"
#include "ecs/Component.hpp"

namespace ion::ecs
{
	Instance::Instance():
		archMap_()
	{
		registerArch(0);
	}

	void Instance::registerArch(std::size_t bitmask)
	{
		assert(!archMap_.contains(bitmask));

		std::vector<ComponentInfo*> c;
		std::size_t size = 0;

		for (std::size_t i = 0; i < bitmask; i++)
		{
			std::size_t b = 1ULL << i;
			if ((bitmask & b) == b)
			{
				ComponentInfo& component = ComponentRegistery::components_.at(i);
				size += component.size;
				c.emplace_back(std::addressof(component));
			}
		}

		archMap_.emplace(std::piecewise_construct, std::forward_as_tuple(bitmask), std::forward_as_tuple(*this, bitmask, size, std::move(c)));
	}

	Arch* Instance::getArchPtr(std::size_t bitmask)
	{
		if (!archMap_.contains(bitmask))
			registerArch(bitmask);
		return std::addressof(archMap_.at(bitmask));
	}
}