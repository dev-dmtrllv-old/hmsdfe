#include "pch.hpp"

#include "engine/Engine.hpp"
#include "engine/SubSystem.hpp"
#include "engine/Game.hpp"

#include "utils/String.hpp"
#include "utils/Pointer.hpp"

#include "test-game/Launcher.hpp"

#include "async/file.hpp"
#include "async/Worker.hpp"
#include "launcher/SharedMemory.hpp"

class Game: public ion::engine::Game
{
public:
	Game(): ion::engine::Game() { }

	Game(const Game&) = delete;
	Game(Game&&) = delete;

	virtual ~Game() override { }

	virtual const char* name() const noexcept override
	{
		return "Test Game";
	}

	virtual void onLoad() override
	{

	}
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
	using namespace ion::launcher;

	try
	{
		SharedMemory::Writer& writer = SharedMemory::Writer::get(L"TEST 1 2 3");

		writer.write("test 1 2 3");
	}
	catch (const std::runtime_error& err)
	{
		MessageBoxA(NULL, err.what(), err.what(), MB_OK);
	}

	return 0;


	// try
	// {
	// 	using namespace ion::engine;

	// 	const Engine& engine = Engine::initialize<::Game>();

	// 	engine.run();

	// 	Engine::terminate();
	// }
	// catch (const std::runtime_error& e)
	// {
	// 	MessageBoxA(NULL, e.what(), e.what(), MB_OK);
	// }

	// return 0;
}