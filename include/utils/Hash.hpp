#pragma once

#include "pch.hpp"

namespace ion::utils
{
	using Hash = std::size_t;

	namespace
	{
		constexpr Hash hashString(const char* str)
		{
			Hash hash = 5381;
			int c = *str;

			while (c)
			{
				hash = ((hash << 5) + hash) * 33 + c;
				c = *str++;
			}

			return hash;
		}
	}

	constexpr Hash hash(const char* str) { return hashString(str); }
	constexpr bool check(Hash& hashStr, const char* str) { return hash(str) == hashStr; }
};