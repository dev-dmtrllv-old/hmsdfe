#include "pch.hpp"

#include "engine/Engine.hpp"
#include "engine/SubSystem.hpp"
#include "engine/Game.hpp"

#include "utils/String.hpp"

#include "test-game/Launcher.hpp"

#include "async/file.hpp"
#include "async/Worker.hpp"
#include "win/wrapper.hpp"

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
	using namespace ion::engine;
	using namespace ion::test_game;

	try
	{
		ion::async::Worker& worker = ion::async::Worker::get();

		// [[maybe_unused]] auto file = ion::async::File("D:\\test.txt");
		// file.read();

		worker.postWork([](void*)
		{
			MessageBoxA(NULL, "working", "working", MB_OK);
		}, [](void*)
		{
			MessageBoxA(NULL, "done", "done", MB_OK);
		});

		MessageBoxA(NULL, "wait", "wait", MB_OK);
		worker.stop();
	}
	catch(const std::runtime_error& e)
	{
		MessageBoxA(NULL, e.what(), e.what(), MB_OK);
	}	

	// Launcher::get().send();

	// const Engine& engine = Engine::initialize<::Game>();

	// engine.run();

	// Engine::terminate();



	return 0;
}