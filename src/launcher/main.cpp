#include "pch.hpp"
#include "launcher/Process.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	using namespace ion::launcher;

	try
	{
		Process gameProcess("test-game.exe");
	}
	catch (const std::runtime_error& e)
	{
		puts(e.what());
		return 1;
	}

	return 0;
}