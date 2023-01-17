#include "utils/String.hpp"

namespace ion::utils
{
	const std::wstring toWString(const char* str)
	{
		std::size_t length = strlen(str) + 1;
		std::wstring wcString(length, L'\0');
		std::size_t numOfConverted = 0;
		mbstowcs_s(&numOfConverted, &wcString[0], length, str, length);
		return wcString;
	}

	const std::wstring toWString(const std::string& str)
	{
		return toWString(str.c_str());
	}

	const std::string toString(const wchar_t* str)
	{
		std::size_t length = wcslen(str) + 1;
		std::string string(length, '\0');
		std::size_t numOfConverted = 0;
		wcstombs_s(&numOfConverted, &string[0], length, str, length);
		return string;
	}

	const std::string toString(const std::wstring& str)
	{
		return toString(str.c_str());
	}
}
