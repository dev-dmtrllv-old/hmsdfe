#include "async/file.hpp"
#include "utils/String.hpp"

namespace ion::async
{
	File::File([[maybe_unused]] const std::string& path):
		path(path)
	{
		auto buf = utils::toWString(this->path);
		fileHandle_ = win::createFile(buf.data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		assert(fileHandle_ != INVALID_HANDLE_VALUE);
	}

	File::~File()
	{
		win::closeHandle(fileHandle_);
	}

	static void onComplete([[maybe_unused]] DWORD err, [[maybe_unused]] DWORD bytesTransfered, [[maybe_unused]] LPOVERLAPPED overlapped)
	{
		[[maybe_unused]] auto i = *(int*)(void*)(&overlapped->hEvent);
		MessageBoxA(NULL, "oki :D", "hihi", MB_OK);
	}

	void File::read()
	{
		OVERLAPPED overlapped = { 0 };
		overlapped.hEvent = (HANDLE)123; // HACK use hEvent as index into tasks array
		
		LARGE_INTEGER fileSize = {};

		if (!GetFileSizeEx(fileHandle_, &fileSize))
			throw std::runtime_error("Could not get file size!");

		buffer_.resize(fileSize.QuadPart + 1);

		ReadFileEx(fileHandle_, buffer_.data(), static_cast<DWORD>(fileSize.QuadPart + 1), &overlapped, onComplete);
		
		
	}
}
