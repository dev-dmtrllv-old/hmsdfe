#pragma once

#include "pch.hpp"

namespace ion::utils::pointer
{
	struct Tag
	{
		unsigned int a : 1 = 0;
		unsigned int b : 1 = 0;
		unsigned int c : 1 = 0;
		unsigned int : 1;
	};

	union ByteTag
	{
		Tag tag;
		std::uint8_t byte;

		ByteTag(Tag tag) noexcept:
			tag(tag)
		{ }

		ByteTag(std::uint8_t byte) noexcept:
			byte(byte)
		{ }
	};

	std::uint8_t tagToByte(const Tag tag) noexcept
	{
		const ByteTag converter(tag);
		return converter.byte;
	}

	Tag byteToTag(const std::uint8_t byte) noexcept
	{
		const ByteTag converter(byte);
		return converter.tag;
	}

	template<typename T>
	class TaggedPtr
	{
	public:
		using ptr_t = std::uintptr_t;

	private:
		static inline const ptr_t ZERO_TAG = tagToByte({ 0, 0, 0 });
		static inline const ptr_t TAG_MASK = tagToByte({ 1, 1, 1 });
		static inline const ptr_t CLEAR_MASK = ~TAG_MASK;

	public:
		// template<typename... Args>
		// static TaggedPtr alloc(Tag tag = ZERO_TAG, Args... args)
		// {
		// 	return TaggedPtr(new T(std::forward<Args>(args)...), tag);
		// }

		TaggedPtr(T* ptr, Tag tag = ZERO_TAG) noexcept:
			ptr_(reinterpret_cast<ptr_t>(ptr) | tagToByte(tag))
		{
			assert(ptr != nullptr);
		}

		TaggedPtr(const TaggedPtr& ptr) = delete;

		TaggedPtr(TaggedPtr&& ptr) noexcept:
			ptr_(nullptr)
		{
			std::exchange(ptr_, ptr);
			assert(ptr_ != nullptr);
		}

		T* clean() const noexcept
		{
			return reinterpret_cast<T*>(ptr_ & CLEAR_MASK);
		}

		T* operator->() const noexcept
		{
			return clean();
		}

		T& operator*() const noexcept
		{
			return *clean();
		}

		T& operator=(T&& other)
		{
			T& val = *clean();
			val = other;
			return val;
		}

		Tag tag() const noexcept
		{
			return byteToTag(static_cast<std::uint8_t>(ptr_ & TAG_MASK));
		}

		void setTag(Tag tag) noexcept
		{
			ptr_ = (ptr_ & CLEAR_MASK) | tagToByte(tag);
		}

	private:
		std::uintptr_t ptr_;
	};
}