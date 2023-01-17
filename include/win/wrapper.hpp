#pragma once

#include "win/error.hpp"

namespace ion::win
{
	template<typename... Args>
	auto wrap(auto callback)
	{
		return [ callback ](Args... args)
		{
			auto result = callback(args...);

			const DWORD lastError = GetLastError();

			if (lastError != ERROR_SUCCESS)
			{
				const std::string msg = error::getLastErrorAsString();
				SetLastError(0);
				throw std::runtime_error(msg);
			}

			return result;
		};
	}

	const auto getModuleHandle = wrap([]() { return ::GetModuleHandle(NULL); });
	const auto createIoCompletionPort = wrap<HANDLE, HANDLE, ULONG_PTR, DWORD>([](HANDLE file, HANDLE port, ULONG_PTR key, DWORD threads) { return ::CreateIoCompletionPort(file, port, key, threads); });
	const auto closeHandle = wrap<HANDLE>([](HANDLE handle){ return ::CloseHandle(handle); });
	const auto createFile = wrap<LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE>([](
			LPCWSTR lpFileName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes,
			HANDLE hTemplateFile
		)
	{
		return ::CreateFile(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile
		);
	});
	const auto readFile = wrap<HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED>([](HANDLE hFile, LPVOID lpBuffer, DWORD toRead, LPDWORD read, LPOVERLAPPED lpOverlapped) { return ::ReadFile(hFile, lpBuffer, toRead, read, lpOverlapped); });
}