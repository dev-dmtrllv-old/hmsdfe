#pragma once

#include "pch.hpp"

namespace ion::utils
{
	const std::wstring toWString(std::string_view str);

	const std::string toString(std::wstring_view str);
}
