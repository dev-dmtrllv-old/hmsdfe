#include "ecs/ArchBufferPool.hpp"

namespace ion::ecs
{
	ArchBuffer::ArchBuffer(std::size_t count, std::size_t archSize):
		buffer_(nullptr),
		ptr_(0),
		archSize_(archSize)
	{
		if (archSize > 0)
		{
			buffer_ = static_cast<char*>(_aligned_malloc(archSize * count, 0x40));
			memset(buffer_, 0, archSize * count);
		}
	}

	ArchBuffer::~ArchBuffer()
	{
		_aligned_free(buffer_);
	}

	ArchBufferPool::ArchBufferPool(std::size_t archSize, std::size_t bufferCapacity):
		archSize_(archSize),
		bufferCapacity_(bufferCapacity),
		bufferSize_(bufferCapacity* archSize),
		ptr_(0),
		buffers_()
	{
		addBuffer();
	}

	void ArchBufferPool::addBuffer()
	{
		ptr_ = buffers_.size();
		buffers_.emplace_back(new ArchBuffer(bufferCapacity_, archSize_));
	}

	Entity ArchBufferPool::alloc()
	{
		const std::size_t bufferIndex = ptr_;
		std::size_t index = buffers_[bufferIndex]->ptr_++;
		if (index >= bufferCapacity_)
			addBuffer();
		return Entity(bufferIndex, index);
	}

	bool ArchBufferPool::free([[maybe_unused]] const Entity& entity)
	{
		// TODO
		return true;
	}

	char* ArchBufferPool::getRaw(const Entity& entity) const
	{
		return buffers_[entity.bufferIndex]->buffer_ + (entity.index * archSize_);
	}

	std::size_t ArchBufferPool::getCount() const
	{
		return (ptr_ * bufferCapacity_) + buffers_[ptr_]->ptr_;	
	}
}