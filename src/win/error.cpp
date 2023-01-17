#include "win/error.hpp"

namespace ion::win::error
{
	std::string getLastErrorAsString()
	{
		std::uint32_t errorMessageID = ::GetLastError();

		if (errorMessageID == 0)
			return std::string();

		LPSTR messageBuffer = nullptr;
		std::uint32_t flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		std::size_t size = FormatMessageA(flags, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		return message;
	}
}