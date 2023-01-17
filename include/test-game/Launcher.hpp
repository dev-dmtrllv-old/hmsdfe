#pragma once

#include "pch.hpp"

namespace ion::test_game
{
	struct Launcher
	{
	public:
		struct ParentProcessInfo
		{
			const std::size_t ppid;
			const std::uint32_t threadID;

			ParentProcessInfo(std::size_t ppid, std::uint32_t threadID):
				ppid(ppid),
				threadID(threadID)
			{ }
		};

		static inline const Launcher& get() noexcept
		{
			try
			{
				static Launcher instance_;
				return instance_;
			}
			catch(const std::runtime_error& e)
			{
				puts(e.what());
				MessageBoxA(NULL, e.what(), "ERROR", MB_ICONERROR | MB_OK);
				std::rethrow_exception(std::current_exception());
			}
		}

	private:
		static std::vector<std::string> parseCommandLineArgs();
		static ParentProcessInfo getParentProcessInfo();

	private:
		Launcher();
		Launcher(const Launcher&) = delete;
		Launcher(Launcher&&) = delete;
		~Launcher();

	public:
		inline bool send(UINT msg = WM_USER, WPARAM wParam = 0, LPARAM lParam = 0) const noexcept 
		{
			return PostThreadMessage(parentProcess_.threadID, msg, wParam, lParam);
		}

		inline const std::vector<std::string>& args() const noexcept { return args_; }

	private:
		const std::vector<std::string> args_;
		const ParentProcessInfo parentProcess_;
		HANDLE thread_;
	};
}