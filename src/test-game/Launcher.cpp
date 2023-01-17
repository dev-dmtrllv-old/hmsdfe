#include "pch.hpp"
#include "test-game/Launcher.hpp"
#include "utils/String.hpp"

namespace ion::test_game
{
	static inline std::string getLastErrorAsString()
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

	std::vector<std::string> Launcher::parseCommandLineArgs()
	{
		std::string commandLine = utils::toString(GetCommandLine());
		std::vector<std::string> args;
		std::size_t lastIndex = 0;
		const std::size_t length = commandLine.length();

		for (std::size_t i = 0; i < length; i++)
		{
			if (commandLine[i] == ' ')
			{
				commandLine[i] = '\0';
				args.emplace_back(&commandLine[lastIndex]);
				lastIndex = i + 1;
			}
		}

		if (commandLine[length - 1] != ' ')
			args.emplace_back(&commandLine[lastIndex]);

		return args;
	}

	Launcher::ParentProcessInfo Launcher::getParentProcessInfo()
	{
		const std::size_t pid = GetCurrentProcessId();
		HANDLE h = NULL;
		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(PROCESSENTRY32);
		h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (Process32First(h, &pe))
		{
			do
			{
				if (pe.th32ProcessID == pid)
				{
					THREADENTRY32 threadEntry;
					ZeroMemory(&threadEntry, sizeof(THREADENTRY32));
					threadEntry.dwSize = sizeof(THREADENTRY32);

					if (Thread32First(h, &threadEntry))
					{
						ParentProcessInfo info(pe.th32ParentProcessID, threadEntry.th32ThreadID);
						CloseHandle(h);
						return info;
					}
					else
					{
						if(GetLastError() == ERROR_NO_MORE_FILES)
							throw std::runtime_error(std::format("Could not get parents main thread ID!\nParent process has no threads!\n {}", getLastErrorAsString().c_str()));	
						throw std::runtime_error(std::format("Could not get parents main thread ID!\n {}", getLastErrorAsString().c_str()));
					}
				}
			} while (Process32Next(h, &pe));
		}

		throw std::runtime_error("Could not get parents process ID!");
	}

	Launcher::Launcher():
		args_(parseCommandLineArgs()),
		parentProcess_(getParentProcessInfo())
	{
		puts("Launcher()");
	}

	Launcher::~Launcher()
	{
		puts("~Launcher()");
	}
}