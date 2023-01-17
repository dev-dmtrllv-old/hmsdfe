#include "pch.hpp"

#include "engine/Engine.hpp"
#include "engine/SubSystem.hpp"
#include "engine/Game.hpp"

#include "utils/String.hpp"

#include "test-game/Launcher.hpp"

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

	Launcher::get().send();

	const Engine& engine = Engine::initialize<::Game>();

	engine.run();

	Engine::terminate();


	return 0;
}