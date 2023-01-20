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

	template<typename T>
	class TaggedPtr
	{
	public:
		using ptr_t = std::uintptr_t;

	private:
		static inline const ptr_t ZERO_MASK = 0b000ULL;
		static inline const ptr_t TAG_MASK = 0b111ULL;
		static inline const ptr_t CLEAR_MASK = ~0b111ULL;

	public:
		TaggedPtr(T* ptr, Tag tag) noexcept:
			ptr_(reinterpret_cast<ptr_t>(ptr))
		{
			assert(ptr != nullptr);
			setTag(tag);
		}

		TaggedPtr(const TaggedPtr& ptr) = delete;

		TaggedPtr(TaggedPtr&& ptr) noexcept:
			ptr_(nullptr)
		{
			std::exchange(ptr_, ptr);
			assert(ptr_ != nullptr);
		}

private:
		T* clean() const noexcept
		{
			return reinterpret_cast<T*>(ptr_ & CLEAR_MASK);
		}

public:
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

		std::uint8_t tagAsByte() const noexcept
		{
			return static_cast<std::uint8_t>(ptr_ & TAG_MASK);
		}

		Tag tag() const noexcept
		{
			std::uint8_t bits = static_cast<std::uint8_t>(ptr_ & TAG_MASK);

			return Tag { 
				.a = bits & 0b001ULL,
				.b = bits & 0b001ULL,
				.c = bits & 0b001ULL,
			};
		}

		void setTag(Tag tag) noexcept
		{
			std::uint8_t bits = 0;
			bits |= (tag.a & 0b001ULL) << 0;
			bits |= (tag.b & 0b001ULL) << 1;
			bits |= (tag.c & 0b001ULL) << 2;
			setTag(bits);
		}

		void setTag(std::uint8_t bits) noexcept
		{
			assert(bits < 8);
			ptr_ = (ptr_ & CLEAR_MASK) | bits;
		}

	private:
		std::uintptr_t ptr_;
	};
}