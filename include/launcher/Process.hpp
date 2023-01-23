#pragma once

#include "pch.hpp"
#include "utils/String.hpp"
#include "win/error.hpp"
#include "launcher/SharedMemory.hpp"

namespace ion::launcher
{
	[[noreturn]] void throwError(std::string_view str)
	{
		throw std::runtime_error(str.data());
	}

	class Process
	{
	private:
		static SECURITY_ATTRIBUTES createSecurityAttributes()
		{
			return {
				.nLength = sizeof(SECURITY_ATTRIBUTES),
				.lpSecurityDescriptor = NULL,
				.bInheritHandle = true
			};
		}

	public:
		Process(std::string&& application):
			application_(std::move(application)),
			securityAttr_(createSecurityAttributes()),
			sharedMem_(&securityAttr_, L"TEST 1 2 3")
		{
			std::wstring args = L"";
			
			ZeroMemory(&procInfo_, sizeof(PROCESS_INFORMATION));

			STARTUPINFO siStartInfo_;
			ZeroMemory(&siStartInfo_, sizeof(STARTUPINFO));
			siStartInfo_.cb = sizeof(STARTUPINFO);
			siStartInfo_.dwFlags |= STARTF_USESTDHANDLES;
			
			const std::wstring exe = utils::toWString(application_);

			if (!CreateProcess(exe.c_str(), args.data(), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo_, &procInfo_))
				throwError("CreateProcess");
		}

		Process(const Process&) = delete;
		Process(Process&&) = delete;
		~Process()
		{
			CloseHandle(procInfo_.hProcess);
			CloseHandle(procInfo_.hThread);
		}

		bool exited() const noexcept { return exited_; }

		HANDLE handle() const noexcept { return procInfo_.hProcess; }
		HANDLE processHandle() const noexcept { return procInfo_.hProcess; }

		void read() { sharedMem_.read(); }

	private:
		const std::string application_;
		SECURITY_ATTRIBUTES securityAttr_;
		PROCESS_INFORMATION procInfo_;

		SharedMemory sharedMem_;
		
		bool exited_ = false;

	};
}