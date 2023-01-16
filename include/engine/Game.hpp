#pragma once

namespace ion::engine
{
	class Game
	{
	public:
		Game() {}
		Game(const Game&) = delete;
		Game(Game&&) = delete;

		virtual ~Game() = 0 {}
		virtual const char* name() const noexcept = 0;

		virtual void onLoad() = 0;
	};
}