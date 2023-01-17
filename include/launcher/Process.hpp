#pragma once

#include "pch.hpp"
#include "utils/String.hpp"

namespace ion::launcher
{
	class Process
	{
	public:
		static inline LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
				case WM_USER:
					puts("got WM_USER msg!");
					break;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		static inline std::string getLastErrorAsString()
		{
			std::uint32_t errorMessageID = ::GetLastError();

			if (errorMessageID == 0)
				return std::string();

			LPSTR messageBuffer = nullptr;
			std::uint32_t flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			size_t size = FormatMessageA(flags, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
			std::string message(messageBuffer, size);
			LocalFree(messageBuffer);
			return message;
		}

		Process(std::string&& application):
			wc_(),
			application_(std::move(application))
		{
			wc_.style = 0;
			wc_.lpfnWndProc = (WNDPROC)WindowProc;
			wc_.cbClsExtra = 0;
			wc_.cbWndExtra = 0;
			wc_.hInstance = GetModuleHandle(NULL);
			wc_.lpszMenuName = L"MainMenu";
			wc_.lpszClassName = L"MainWndClass";

			RegisterClass(&wc_);

			ZeroMemory(&si_, sizeof(STARTUPINFO));
			ZeroMemory(&pi_, sizeof(PROCESS_INFORMATION));

			const auto appName = utils::toWString(application_.c_str());
			std::wstring commandLine = L"";
			const uint32_t creationFlags = 0;

			printf("Creating sub process %s\n", application_.c_str());

			if (!CreateProcess(appName.c_str(), commandLine.data(), NULL, NULL, FALSE, creationFlags, NULL, NULL, &si_, &pi_))
			{
				throw std::runtime_error(std::format("Could not create sub process!\n{}", getLastErrorAsString().c_str()));
			}

			BOOL ret;
			MSG msg;

			while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
			{
				if (ret == -1)
				{
					throw std::runtime_error("GetMessageError!");
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			printf("sub process %s done!\n", application_.c_str());
		}

		Process(const Process&) = delete;
		Process(Process&&) = delete;
		~Process()
		{
			CloseHandle(pi_.hProcess);
			CloseHandle(pi_.hThread);
		}

	private:
		WNDCLASS wc_;
		const std::string application_;
		STARTUPINFO si_;
		PROCESS_INFORMATION pi_;
	};
}