#pragma once

#include "pch.hpp"

namespace ion::utils
{
	const std::wstring toWString(const char* str);
	const std::wstring toWString(const std::string& str);

	const std::string toString(const wchar_t* str);
	const std::string toString(const std::wstring& str);
}
