#include "ecs/Arch.hpp"
#include "ecs/Component.hpp"
#include "ecs/Instance.hpp"

#ifndef ARCH_BUFFER_CAPACITY
#define ARCH_BUFFER_CAPACITY 16
#endif
namespace ion::ecs
{
	Arch::Arch(Instance& instance, std::size_t bitmask, std::size_t archSize, std::vector<ComponentInfo*>&& components):
		bitmask(bitmask),
		archSize(archSize),
		instance_(instance),
		bufferPool_(archSize, ARCH_BUFFER_CAPACITY),
		components_(components),
		offsets_(components.size(), { 0, 0 })
	{
		std::size_t offset = 0;
		for (std::size_t i = 0, l = components_.size(); i < l; i++)
		{
			const std::size_t s = components_.at(i)->size;
			offsets_[i].size = s;
			offsets_[i].offset = offset;
			offset += s;
		}
	}

	Arch::~Arch()
	{

	}

	Entity Arch::alloc()
	{
		return bufferPool_.alloc();
	}

	void Arch::free(const Entity& entity)
	{
		bufferPool_.free(entity);
	}

	Arch* Arch::addComponentToEntity(Entity& entity, const ComponentInfo& component)
	{
		const ArchArm& nextArch = getNext(component);
		const Entity newEntity = nextArch.arch->alloc();

		char* toBuffer = nextArch.arch->bufferPool_.getRaw(newEntity);
		char* fromBuffer = bufferPool_.getRaw(entity);
		const auto split = nextArch.splitOffset;

		if (split == archSize)
		{
			memcpy(toBuffer, fromBuffer, archSize);
		}
		else if (split == 0)
		{
			memcpy(toBuffer + component.size, fromBuffer, archSize);
		}
		else
		{
			memcpy(toBuffer, fromBuffer, split);
			memcpy(toBuffer + split + component.size, fromBuffer + split, archSize - split);
		}

		free(entity);
		entity = newEntity;

		return nextArch.arch;
	}

	void* Arch::getComponentRaw(const Entity& entity, std::size_t bitmask) const
	{
		return bufferPool_.getRaw(entity) + getComponentOffset(bitmask).offset;
	}

	const ComponentOffset& Arch::getComponentOffset(const ComponentInfo& component) const
	{
		return getComponentOffset(component.bitmask);
	}

	const ComponentOffset& Arch::getComponentOffset(std::size_t bitmask) const
	{
		for (std::size_t i = 0, l = offsets_.size(); i < l; i++)
		{
			if (components_[i]->bitmask == bitmask)
			{
				return offsets_[i];
			}
		}
		throw std::runtime_error("Could not get component offset!");
	}

	const ArchArm& Arch::getNext(const ComponentInfo& component)
	{
		for (const auto& a : next_)
		{
			if (a->bitmask == component.bitmask)
			{
				return *a;
			}
		}

		const auto b = bitmask | component.bitmask;

		Arch* newArch = instance_.getArchPtr(b);
		const auto& offset = newArch->getComponentOffset(component);
		return *next_.emplace_back(new ArchArm(newArch, component.bitmask, offset.offset, component.size));
	}

	const ArchArm& Arch::getPrev(const ComponentInfo& component)
	{
		// TODO:
		throw "!!! TODO !!!";
	}

}