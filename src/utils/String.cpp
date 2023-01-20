#include "utils/String.hpp"

namespace ion::utils
{
	const std::wstring toWString(std::string_view str)
	{
		const std::size_t length = str.length() + 1;
		std::wstring wcString(length, L'\0');
		std::size_t numOfConverted = 0;
		mbstowcs_s(&numOfConverted, &wcString[0], length, str.data(), length);
		return wcString;
	}

	const std::string toString(std::wstring_view str)
	{
		std::size_t length = str.length() + 1;
		std::string string(length, '\0');
		std::size_t numOfConverted = 0;
		wcstombs_s(&numOfConverted, &string[0], length, str.data(), length);
		return string;
	}
}
