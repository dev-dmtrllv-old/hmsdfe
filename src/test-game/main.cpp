#include "pch.hpp"
#include "engine/Engine.hpp"
#include "engine/SubSystem.hpp"
#include "engine/Game.hpp"

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

int main()
{
	using namespace ion::engine;

	const Engine& engine = Engine::initialize<::Game>();

	engine.run();

	Engine::terminate();

	return 0;
}